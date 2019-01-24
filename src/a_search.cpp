#include "a_search.h"
#include <cmath>
#include <algorithm>
#include <set>


Search::Search(Model &model): m_Model(model) {
    /*Set start and end points*/
    //end = m_Model.Nodes()[m_Model.Ways()[6].nodes[0]];
    //start = m_Model.Nodes()[m_Model.Ways()[0].nodes[0]];

    bool user_input = true;

    start.x = m_Model.start_x/m_Model.MetricScale();
    start.y = m_Model.start_y/m_Model.MetricScale();
    end.x = m_Model.end_x/m_Model.MetricScale();
    end.y = m_Model.end_y/m_Model.MetricScale();

    start = m_Model.FindClosestNode(start);
    end = m_Model.FindClosestNode(end);

    CalculateHValue(end);
    m_Model.start_position = start;
    m_Model.end_position = end;
    m_Model.parents.assign(m_Model.Nodes().size() ,-1);
    
    //Call A* algorithm
    m_Model.path = AStar();

}

std::vector<Model::Node> Search::AStar(){

    // Initialize open_list with starting node.
    std::vector<Model::Node> open_list;
    m_Model.Nodes()[start.index].visited = true;
    open_list.emplace_back(start);
    Model::Node current_node = open_list.back();

    // Expand nodes until you reach the goal. Use heuristic to prioritize what node to open first.
    while (open_list.size() > 0) {
        //Select the best node to explore next.
        current_node = NextNode(open_list, current_node);

        //Check if the node selected is the goal.
        if(current_node.x == end.x && current_node.y == end.y ) {
            std::cout<<"Hooray for you!"<<std::endl;
            std::vector<Model::Node> path_found = CreatePathFound(current_node); 
            std::cout<<"distance: " << distance <<"\n";
            return path_found;
        }
        AddNeighbors(open_list, current_node);

    }  //open_list while loop

    std::cout<<"Didn't find it!"<<std::endl;
    return {};
}

void Search::AddNeighbors(std::vector<Model::Node> &open_list, Model::Node current_node) {
    //Expand the current node (add all unvisited neighbors to the open list)
    std::vector<Model::Node> neighbors = FindNeighbors(current_node);

    for (auto neighbor : neighbors) {
        m_Model.parents[neighbor.index] = current_node.index;
        neighbor.g_value = current_node.g_value + current_node.distance(neighbor);

        //Add the neighbor to the open list.
        open_list.emplace_back(neighbor);
        m_Model.Nodes()[neighbor.index].visited = true;
    }
}

Model::Node Search::NextNode(std::vector<Model::Node>&open_list, Model::Node current_node) {
    std::sort(open_list.begin(), open_list.end(), [](const auto &_1st, const auto &_2nd) {
        return _1st.h_value + _1st.g_value < _2nd.h_value + _2nd.g_value;
    });

    Model::Node lowest_node = open_list.front();
    open_list.erase(open_list.begin());
    return lowest_node;
}


void Search::CalculateHValue(Model::Node end) {
    float h_value;
    for(auto &node: m_Model.Nodes()) {
        h_value = std::sqrt(std::pow((end.x - node.x),2)+ std::pow((end.y - node.y),2));
        node.h_value = h_value;
    }
}


Model::Node Search::FindNeighbor(Model::Way way, Model::Node currentPosition){
    Model::Node closest = {};

    for(int node_index : way.nodes) {
        Model::Node node = m_Model.Nodes()[node_index];
        if (node.id != currentPosition.id && !node.visited)
            if(currentPosition.distance(node) < currentPosition.distance(closest))
                closest = node;
    }

    return closest;
}

std::vector<Model::Node > Search::FindNeighbors(Model::Node currentPosition) {
    std::vector<Model::Node > neighbors= {};

    for(auto way_num : currentPosition.way_nums) {
        Model::Node new_neighbor = FindNeighbor(m_Model.Ways()[way_num], currentPosition);
        if(new_neighbor.id != "")
            neighbors.emplace_back(new_neighbor);

    }
    return neighbors;
}

std::vector<Model::Node> Search::CreatePathFound(Model::Node current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<Model::Node> path_found;
    Model::Node parent;

    while (current_node.x != start.x && current_node.y != start.y) {
        path_found.push_back(current_node);
        int parent_index = m_Model.parents[current_node.index];
        parent = m_Model.Nodes()[parent_index];
        distance += current_node.distance(parent);
        current_node = parent;
    }
    path_found.push_back(current_node);
    return path_found;
}

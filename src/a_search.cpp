#include "a_search.h"
#include <cmath>
#include <algorithm>
#include <set>


Search::Search(Model &model):
    m_Model(model)
{
    /*Set start and end points*/
    //end = m_Model.Nodes()[m_Model.Ways()[6].nodes[0]];
    //start = m_Model.Nodes()[m_Model.Ways()[0].nodes[0]];

    bool user_input = true;

    if (user_input) {
        start.x = m_Model.start_x/m_Model.MetricScale();
        start.y = m_Model.start_y/m_Model.MetricScale();
        end.x = m_Model.end_x/m_Model.MetricScale();
        end.y = m_Model.end_y/m_Model.MetricScale();

        start = m_Model.FindClosestNode(start);
        end = m_Model.FindClosestNode(end);
        
        std::cout << "Start x: " << start.x << "\n";
        std::cout << "Start y: " << start.y << "\n";
        std::cout << "Start index: " << start.index << "\n";
        std::cout << "Start id: " << start.id << "\n";
        std::cout << "End x: " << end.x << "\n";
        std::cout << "End y: " << end.y << "\n";
        std::cout << "End index: " << end.index << "\n";
        std::cout << "End id: " << end.id << "\n";
    } else {
    
        end = m_Model.Nodes()[m_Model.Ways()[6].nodes[0]];
        start = m_Model.Nodes()[m_Model.Ways()[0].nodes[0]];
        std::cout << "Working Start node: " << m_Model.Nodes()[33].way_nums[0] << "\n";
        std::cout << "Working End node: " << m_Model.Nodes()[87].way_nums[0] << "\n";
        std::cout << "Working Start way type_hwy: " << m_Model.Ways()[m_Model.Nodes()[33].way_nums[0]].type_highway << "\n";
        std::cout << "Working End way type_hwy: " << m_Model.Ways()[m_Model.Nodes()[87].way_nums[0]].type_highway << "\n";

        std::cout << "Not Working Start node info: " << "\n";
        for (auto way_num: m_Model.Nodes()[6973].way_nums) {
            std::cout << "Not Working Start way type_hwy: " << m_Model.Ways()[way_num].type_highway << "\n";
        }
        std::cout << "Not Working End node info: " << "\n";
        for (auto way_num: m_Model.Nodes()[6629].way_nums) {
            std::cout << "Not Working End way type_hwy: " << m_Model.Ways()[way_num].type_highway << "\n";
        }
        std::cout << "Start id: " << start.id << "\n";
        std::cout << "End id: " << end.id << "\n";
    }


    Calculate_H_Value(end);
    m_Model.start_position = start;
    m_Model.end_position = end;
    m_Model.parents.assign(m_Model.Nodes().size() ,-1);
    
    //Call A* algorithm

    m_Model.path = A_Star();
    //m_Model.path.emplace_back(start);
    //m_Model.path.emplace_back(end);

}

std::vector<Model::Node> Search::A_Star(){

    /*
    TODO:
    1. Create a new object of type Node.
    2. Assign the node member to the start node.
    3. Initialize an empty vector, for the open nodes.
    4. Insert into the open list, the Node.
    5. Initilize the g value.
    6. Calculate the h value for all the nodes.
    7. As long as there is nodes in the open list, keep looking at the nodes
    8. Call a function that give you the best next node to move based on the heuristic
    9. Check if the selected node is the goal, if it is then return the path found adding the nodes in the openlist and the current node.
    10. If the current node is not the goal, then look for its neighbor.
    11. Concatenate the parents seen to the current.
    12. Push the nodes into the open list of nodes.
    13. Increment the g value.
    14. If you ran out of nodes without finding a path, then there is no path.
    */

    std::vector<Model::Node> openlist;
    m_Model.Nodes()[start.index].visited = true;
    openlist.emplace_back(start);
    Model::Node current_node = openlist.back();

    /*Expand nodes until you reach the goal. Use heuristic to prioritize what node to open first*/
    while (openlist.size() > 0)
    {
        //Select the best node to explore next.
        current_node = Next_Node(openlist, current_node);

        //Check if the node selected is the goal.
        if(current_node.x == end.x && current_node.y == end.y )
        {
            std::cout<<"Hooray for you!"<<std::endl;
            std::vector<Model::Node> path_found = Create_Path_Found(current_node); 
            std::cout<<"distance: " << distance <<"\n";
            return path_found;
        }
        AddNeighbors(openlist, current_node);

    }  //openlist while loop


    std::cout<<"Didn't find it!"<<std::endl;

    return {};
}

void Search::AddNeighbors(std::vector<Model::Node> &openlist, Model::Node current_node) {
    //Expand the current node (add all unvisited neighbors to the open list)
    std::vector<Model::Node> neighbors = Find_Neighbors(current_node);

    for (auto neighbor : neighbors)
    {
        m_Model.parents[neighbor.index] = current_node.index;
        neighbor.g_value = current_node.g_value + current_node.distance(neighbor);

        //Add the neighbor to the open list.
        openlist.emplace_back(neighbor);
        m_Model.Nodes()[neighbor.index].visited = true;
    }
}

Model::Node Search::Next_Node(std::vector<Model::Node>&openlist, Model::Node current_node){
    std::sort(openlist.begin(), openlist.end(), [](const auto &_1st, const auto &_2nd){
        return _1st.h_value + _1st.g_value < _2nd.h_value + _2nd.g_value;
    });

    Model::Node lowest_node = openlist.front();
    openlist.erase(openlist.begin());
    return lowest_node;
}


void Search::Calculate_H_Value(Model::Node end)
{
    float h_value;
    for(auto &node: m_Model.Nodes()) {
        h_value = std::sqrt(std::pow((end.x - node.x),2)+ std::pow((end.y - node.y),2));
        node.h_value = h_value;
    }
}


Model::Node Search::Find_Neighbor(Model::Way way, Model::Node currentPosition){
    Model::Node closest = {};

    for(int node_index : way.nodes)
    {
        Model::Node node = m_Model.Nodes()[node_index];
        if (node.id != currentPosition.id && !node.visited)
            if(currentPosition.distance(node) < currentPosition.distance(closest))
                closest = node;
    }

    return closest;
}

std::vector<Model::Node > Search::Find_Neighbors(Model::Node currentPosition)
{
    std::vector<Model::Node > neighbors= {};

    for(auto way_num : currentPosition.way_nums)
    {
        Model::Node new_neighbor = Find_Neighbor(m_Model.Ways()[way_num], currentPosition);
        if(new_neighbor.id != "")
            neighbors.emplace_back(new_neighbor);

    }
    return neighbors;
}

std::vector<Model::Node> Search::Create_Path_Found(Model::Node current_node) {
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

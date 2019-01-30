#include "a_search.h"
#include <cmath>
#include <algorithm>
#include <set>


Search::Search(SearchModel &model): m_Model(model) {
    //Call A* algorithm
    m_Model.path = AStar();
}

std::vector<SearchModel::Node> Search::AStar(){

    // Initialize open_list with starting node.
    m_Model.start_node.visited = true;
    open_list.emplace_back(&m_Model.start_node);
    SearchModel::Node * current_node = nullptr;

    // Expand nodes until you reach the goal. Use heuristic to prioritize what node to open first.
    while (open_list.size() > 0) {
        //Select the best node to explore next.
        current_node = NextNode();

        //Check if the node selected is the goal.
        if(current_node->x == m_Model.end_node.x && current_node->y == m_Model.end_node.y ) {
            std::vector<SearchModel::Node> path_found = CreatePathFound(current_node);
            std::cout<<"Hooray for you!"<<std::endl;
            std::cout<<"Distance: " << distance << " meters. \n";
            return path_found;
        }
        AddNeighbors(current_node);
    }

    std::cout<<"Didn't find it!"<<std::endl;
    return {};
}

void Search::AddNeighbors(SearchModel::Node * current_node) {
    //Expand the current node (add all unvisited neighbors to the open list)
    current_node->FindNeighbors();

    for (auto neighbor : current_node->neighbors) {
        neighbor->parent = current_node;
        neighbor->g_value = current_node->g_value + current_node->distance(*neighbor);

        //Add the neighbor to the open list.
        open_list.emplace_back(neighbor);
        neighbor->visited = true;
    }
}

SearchModel::Node * Search::NextNode() {
    std::sort(open_list.begin(), open_list.end(), [](const auto &_1st, const auto &_2nd) {
        return _1st->h_value + _1st->g_value < _2nd->h_value + _2nd->g_value;
    });

    SearchModel::Node * lowest_node = open_list.front();
    open_list.erase(open_list.begin());
    return lowest_node;
}

std::vector<SearchModel::Node> Search::CreatePathFound(SearchModel::Node * current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<SearchModel::Node> path_found;
    SearchModel::Node parent;

    while (current_node->parent != nullptr) {
        path_found.push_back(*current_node);
        parent = *(current_node->parent);
        distance += current_node->distance(parent);
        current_node = current_node->parent;
    }
    path_found.push_back(*current_node);
    distance *= m_Model.MetricScale();
    return path_found;
}

#include "search.h"
#include <cmath>
#include <algorithm>
#include <set>


Search::Search(SearchModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    start_node = &m_Model.FindClosestNode(start_x, start_y);
    end_node = &m_Model.FindClosestNode(end_x, end_y);
    CalculateHValues();
}

void Search::AStar() {
    // Initialize open_list with starting node.
    start_node->visited = true;
    open_list.emplace_back(start_node);
    SearchModel::Node *current_node = nullptr;

    // Expand nodes until you reach the goal. Use heuristic to prioritize what node to open first.
    while (open_list.size() > 0) {
        // Select the best node to explore next.
        current_node = NextNode(); 
        // Check if the node selected is the goal.
        if(current_node->distance(*end_node) == 0) {
            // Set the model path variable with the path found.
            m_Model.path = ConstructFinalPath(current_node);
            return;
        }
        AddNeighbors(current_node);
    }
}

void Search::AddNeighbors(SearchModel::Node *current_node) {
    // Expand the current node (add all unvisited neighbors to the open list)
    current_node->FindNeighbors();

    for (auto neighbor : current_node->neighbors) {
        neighbor->parent = current_node;
        neighbor->g_value = current_node->g_value + current_node->distance(*neighbor);

        // Add the neighbor to the open list.
        open_list.emplace_back(neighbor);
        neighbor->visited = true;
    }
}

SearchModel::Node *Search::NextNode() {
    std::sort(open_list.begin(), open_list.end(), [](const auto &_1st, const auto &_2nd) {
        return _1st->h_value + _1st->g_value < _2nd->h_value + _2nd->g_value;
    });

    SearchModel::Node *lowest_node = open_list.front();
    open_list.erase(open_list.begin());
    return lowest_node;
}

void Search::CalculateHValues() {
    float h_value;
    for (auto &node: m_Model.SNodes()) {
        h_value = std::sqrt(std::pow((end_node->x - node.x), 2)+ std::pow((end_node->y - node.y), 2));
        node.h_value = h_value;
    }
}

std::vector<SearchModel::Node> Search::ConstructFinalPath(SearchModel::Node *current_node) {
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

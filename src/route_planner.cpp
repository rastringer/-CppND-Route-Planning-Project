#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
	start_x *= 0.01;
	start_y *= 0.01; 
	end_x *= 0.01; 
	end_y *= 0.01;

	start_node = &m_Model.FindClosestNode(start_x, start_y);
	end_node = &m_Model.FindClosestNode(end_x, end_y);
}

void RoutePlanner::AStarSearch() {
	start_node->visited = true;
	open_list.push_back(start_node);
	RouteModel::Node *current_node = nullptr;
	while (open_list.size() > 0) {
		// Pick best node to explore next
		current_node = NextNode();
		// Check if selected node is the goal
		if (current_node->distance(*end_node) == 0) {
			// Set model path with path found
			m_Model.path = ConstructFinalPath(current_node);
			return;
		}
		AddNeighbors(current_node);
	}
}

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {

	std::vector<RouteModel::Node> path_found;
	distance = 0.0f;
	RouteModel::Node parent;
	// iterate through node parents until node with parent == nullptr, this is the start node
	while (current_node->parent != nullptr) {
		// push each node in iteration to path_found vector
		path_found.push_back(*current_node);
		// add distance between node and parent class to distance variable
		parent = *(current_node->parent);
		distance += current_node->distance(parent);
		current_node = current_node->parent;
	}
	path_found.push_back(*current_node);
	distance *= m_Model.MetricScale();
	return path_found;
}

/*
NextNode sorts the list of open nodes
in the search and returns the node with the 
lowest f-value 
*/

RouteModel::Node *RoutePlanner::NextNode() {
	// Sort open_list by f-value (sum of h-value, g-value)
	std::sort(open_list.begin(), open_list.end(), [](const auto &_1st, const auto &_2nd) {
		return _1st->h_value + _1st->g_value < _2nd->h_value + _2nd->g_value;
		});
	// Copy the pointer to the node with lowest f-value
	RouteModel::Node *lowest_node = open_list.front();
	// Delete node pointer from open_list
	open_list.erase(open_list.begin());
	return lowest_node;
}

// CalculateHValue calculates the h-value for a given node
float RoutePlanner::CalculateHValue(RouteModel::Node *node) {
	return node->distance(*end_node);
}

// AddNeighbors adds new nodes to the search
void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
	// Expand current node:
	current_node->FindNeighbors();

	for (auto neighbor : current_node->neighbors) {
		neighbor->parent = current_node;
		neighbor->g_value = current_node->g_value + current_node->distance(*neighbor);
		neighbor->h_value = CalculateHValue(neighbor);

		// Add neighbor to open list
		open_list.push_back(neighbor);
		neighbor->visited = true;
	}
}
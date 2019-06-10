#pragma once

#include "model.h"
#include <limits>
#include <cmath>
#include <unordered_map>
#include <iostream>
#include <vector>

class RouteModel : public Model {

  public:
    class Node : public Model::Node {
      public:
		Node * parent = nullptr;
		float h_value = std::numeric_limits<float>::max();
		float g_value = 0.0;
		bool visited = false;
		std::vector<Node *> neighbors;
		void FindNeighbors();
        
		float distance(Node other) const {
			return std::sqrt(std::pow((x - other.x), 2) + std::pow((y - other.y), 2));
		}

        Node(){}
        Node(int idx, RouteModel * search_model, Model::Node node) : Model::Node(node), parent_model(search_model), index(idx) {}
		
				
      private:
        int index;
        RouteModel * parent_model = nullptr;
		Node * FindNeighbor(std::vector<int> node_indices);
		
	};
    
    RouteModel(const std::vector<std::byte> &xml);
	// Getter method to return vector of m_Nodes
	auto &SNodes() { return m_Nodes; };
	std::vector<Node> path; // This variable will eventually store the path that is found by the A* search.
	auto &GetNodeToRoadMap() { return node_to_road; }
	Node& FindClosestNode(float x, float y);


  private:
    std::vector<Node> m_Nodes;
	std::unordered_map <int, std::vector<const Model::Road*>> node_to_road;
	void CreateNodeToRoadHashmap();
};

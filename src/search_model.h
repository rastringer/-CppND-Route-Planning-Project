#pragma once

#include <limits>
#include <cmath>
#include "model.h"
#include <unordered_map>

class SearchModel : public Model {

    public:
        class Node : public Model::Node {
            public:

                int index;
                Node * parent = nullptr;
                float h_value = std::numeric_limits<float>::max();
                float g_value = 0.0;
                bool visited = false;
                std::vector<Node *> neighbors;

                Node * FindNeighbor(std::vector<int> node_indices);
                void FindNeighbors();
                float distance(Node other) {
                    return std::sqrt(std::pow((x - other.x),2) + std::pow((y - other.y),2));
                }

                Node(){}
                Node(int idx, SearchModel * search_model, Model::Node node) : Model::Node(node), parent_model(search_model) {index = idx;}

            private:
                SearchModel * parent_model = nullptr;
        };


        SearchModel(const std::vector<std::byte> &xml, float start_x, float start_y, float end_x, float end_y);


        Node &FindClosestNode(float x, float y);
        void CalculateHValues();
        void CreateNodeToRoadHashmap();
        auto &SNodes() { return m_Nodes; }
        
        Node start_node;
        Node end_node;
        std::vector<Node> path;
    
    private:
        std::unordered_map<int, std::vector<const Model::Road *>> node_to_road;
        std::vector<Node> m_Nodes;
};
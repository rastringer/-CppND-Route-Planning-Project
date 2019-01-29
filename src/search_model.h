#pragma once

#include <limits>
#include <cmath>
#include "model.h"

class SearchModel : public Model {

    public:
        struct Node : public Model::Node {
            Node * FindNeighbor(SearchModel & model, int way_num);
            void FindNeighbors(SearchModel & model);

            std::string id = "";
            std::vector<int> way_nums;
            int index;
            int parent_index = -1;
            float h_value = std::numeric_limits<float>::max();
            float g_value = 0.0;
            bool visited = false;
            std::vector<Node *> neighbors;

            float distance(Node other) {
                return std::sqrt(std::pow((x - other.x),2) + std::pow((y - other.y),2));
            }

        };

        struct Way : public Model::Way {
            std::vector<std::string> id_nodes;
            std::string type_highway;
        };

        SearchModel(const std::vector<std::byte> &xml, float start_x, float start_y, float end_x, float end_y);
        Node &FindClosestNode(float x, float y);
        void CalculateHValues(Node end_node);
        auto &SNodes() { return m_Nodes; }


        double min_x;
        double min_y;

        Node start_node;
        Node end_node;
        std::vector<Node> path;
        std::vector<int> parents;
        std::vector<Node> m_Nodes;

};
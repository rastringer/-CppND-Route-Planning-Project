#include "search_model.h"

SearchModel::SearchModel(const std::vector<std::byte> &xml, float start_x, float start_y, float end_x, float end_y) 
    : Model(xml)
{
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;
    start_node = FindClosestNode(start_x, start_y);
    end_node = FindClosestNode(end_x, end_y);
    CalculateHValues(end_node);
    
    // Create SearchModel nodes.
    for (Node node : this->Nodes()) {
        m_Nodes.emplace_back(Node(node));
    }
}


SearchModel::Node * SearchModel::Node::FindNeighbor(SearchModel & model, int way_num){
    SearchModel::Way way = model.Ways()[way_num];
    Node * closest_node = nullptr;
    Node node;

    for(int node_index : way.nodes) {
        node = model.SNodes()[node_index];
        if (node.id != this->id && !node.visited) {
            if(closest_node == nullptr || this->distance(node) < this->distance(*closest_node)) {
                closest_node = &model.SNodes()[node_index];
            }
        }
    }
    return closest_node;
}


void SearchModel::Node::FindNeighbors(SearchModel & model) {
    for(auto way_num : this->way_nums) {
        Model::Node * new_neighbor = this->FindNeighbor(model, way_num);
        if(new_neighbor) {
            this->neighbors.emplace_back(new_neighbor);
        }
    }
}


void SearchModel::CalculateHValues(SearchModel::Node end) {
    float h_value;
    for(auto &node: this->Nodes()) {
        h_value = std::sqrt(std::pow((end.x - node.x),2)+ std::pow((end.y - node.y),2));
        node.h_value = h_value;
    }
}


SearchModel::Node & SearchModel::FindClosestNode(float x, float y) {
    Node input;
    input.x = x;
    input.y = y;
    float min_dist = std::numeric_limits<float>::max();
    float dist;
    int closest_idx;
    bool on_road = false;
    for (const auto &node: this->SNodes()) {
        // Search through ways that this node is a part of, 
        // and check that at least one is a road.
        for (auto way_num: node.way_nums) {
            std::string type_highway = m_Ways[way_num].type_highway;
            if (!type_highway.empty() && type_highway != "footway") {
                on_road = true;
                break;
            }
        }
        if (on_road && node.x > 0 && node.y > 0) {
            dist = input.distance(node);
            if (dist < min_dist) {
                closest_idx = node.index;
                min_dist = dist;
            }
        }
        on_road = false;
    }
    return this->SNodes()[closest_idx];
}
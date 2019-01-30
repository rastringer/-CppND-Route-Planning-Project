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
    int counter = 0;
    for (Model::Node node : this->Nodes()) {
        m_Nodes.emplace_back(Node(counter, this, node));
        counter++;
    }
}


void SearchModel::CreateNodeToRoadHashmap() {
    for (const Model::Road & road : this->Roads()) {
        if (road.type != Model::Road::Type::Footway) {
            for(int node_idx : this->Ways()[road.way].nodes) {
                if (this->node_to_road.find(node_idx) == this->node_to_road.end()) {
                    this->node_to_road[node_idx] = std::vector<const Model::Road *> ();
                }
                this->node_to_road[node_idx].push_back(&road);
            }
        }
    }
}


SearchModel::Node * SearchModel::Node::FindNeighbor(SearchModel & model, std::vector<int> node_indices){
    Node * closest_node = nullptr;
    Node node;

    for(int node_index : node_indices) {
        node = model.SNodes()[node_index];
        // Fix the distance problem below
        if (this->distance(node) != 0 && !node.visited) {
            if(closest_node == nullptr || this->distance(node) < this->distance(*closest_node)) {
                closest_node = &model.SNodes()[node_index];
            }
        }
    }
    return closest_node;
}


void SearchModel::Node::FindNeighbors(SearchModel & model) {
    for(auto & road : this->parent_model->node_to_road[this->index]) {
        SearchModel::Node * new_neighbor = this->FindNeighbor(model, this->parent_model->Ways()[road->way].nodes);
        if(new_neighbor) {
            this->neighbors.emplace_back(new_neighbor);
        }
    }
}


void SearchModel::CalculateHValues(SearchModel::Node end) {
    float h_value;
    for(auto &node: this->SNodes()) {
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

    for (const Model::Road &road: this->Roads()) {
        if (road.type != Model::Road::Type::Footway) {
            for(int node_idx : this->Ways()[road.way].nodes) {
                dist = input.distance(this->SNodes()[node_idx]);
                if (dist < min_dist) {
                    closest_idx = node_idx;
                    min_dist = dist;
                }
            }
        }
    }

    return this->SNodes()[closest_idx];
}
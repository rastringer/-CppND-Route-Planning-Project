#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include "model.h"


class Search
{
public:
    Search( Model &model );
    float GetDistance() const {return distance;};
    void AddNeighbors(Model::Node current_node);
    Model::Node FindNeighbor(Model::Way way, Model::Node currentPosition);
    Model::Node NextNode();
    std::vector<Model::Node> AStar();
    std::vector<Model::Node> FindNeighbors(Model::Node currentPosition);
    std::vector<Model::Node> CreatePathFound(Model::Node);
    
private:
    Model &m_Model;
    float distance;
    std::vector<Model::Node> open_list;

};

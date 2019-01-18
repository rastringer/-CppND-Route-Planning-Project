#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include "model.h"


class Search
{
public:
    Search( Model &model );
    float GetDistance() const { return distance; };
    void AddNeighbors(std::vector<Model::Node> &openList, Model::Node current_node);
    void Calculate_H_Value(Model::Node end);
    Model::Node Find_Neighbor(Model::Way way, Model::Node currentPosition);
    Model::Node Next_Node(std::vector<Model::Node>&openlist, Model::Node current_node);
    std::vector<Model::Node> A_Star();
    std::vector<Model::Node> Find_Neighbors(Model::Node currentPosition);
    std::vector<Model::Node> Create_Path_Found(Model::Node);

    Model::Node start;
    Model::Node end;
    
private:
    Model &m_Model;
    float distance;

};

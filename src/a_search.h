#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include "model.h"


class Search
{
public:
    Search( Model &model );
    std::vector<Model::Node> A_Star(std::vector<Model::Node>&openlist);
    void Calculate_H_Value(Model::Node end);
    std::vector<Model::Node> Find_Neighbors(Model::Node currentPosition);
    Model::Node Find_Neighbor(Model::Way way, Model::Node currentPosition);
    Model::Node Next_Node(std::vector<Model::Node>&openlist, Model::Node current_node);
    void AddNeighbors(std::vector<Model::Node> &openList, Model::Node current_node);
    float GetDistance() const { return distance; };

    Model::Node start;
    Model::Node end;
    
    


private:
    Model &m_Model;
    float distance;

};

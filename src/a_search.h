#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include "model.h"
#include "open_node.h"


class Search
{
public:
    Search( Model &model );
    std::vector<Model::Node> A_Star(std::vector<OpenNode>&openlist);
    void Calculate_H_Value(Model::Node end);
    std::vector<Model::Node> Find_Neighbors(Model::Node currentPosition);
    Model::Node Find_Neighbor(Model::Way way, Model::Node currentPosition);
    OpenNode Next_Node(std::vector<OpenNode>&openlist, OpenNode current_node);
    void AddNeighbors(std::vector<OpenNode> &openList, OpenNode current_node);
    float GetDistance() const { return distance; };

    Model::Node start;
    Model::Node end;
    
    


private:
    Model &m_Model;
    float distance;

};

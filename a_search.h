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
    float Calculate_distance(Model::Node current_node, Model::Node other_node);    
    std::vector<Model::Node> Find_Neighbors(Model::Node currentPosition);
    OpenNode Next_Node(std::vector<OpenNode>&openlist, float gValue, OpenNode current_node);

    Model::Node start;
    Model::Node end;
    
    


private:
    Model &m_Model;

};

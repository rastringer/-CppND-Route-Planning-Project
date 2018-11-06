#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include "model.h"


class Search
{
public:
    Search( Model &model );
    bool A_Star();
    void Calculate_H_Value(Model::Node end);
    void Sort_Nodes(std::vector<Model::Node> nodes);
    std::vector<int> Find_Neighbors(Model::Node currentPosition);
    Model::Node Next_Node(std::vector<int> &neighbors, float gValue);

    Model::Node start;
    Model::Node end;
    


private:
    Model &m_Model;

};

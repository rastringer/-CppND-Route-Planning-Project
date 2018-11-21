#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include "model.h"


class OpenNode
{
public:
    OpenNode();
    Model::Node node;
    std::vector<Model::Node> parents;	
//float g_value;
};

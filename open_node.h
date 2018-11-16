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


bool operator==(const Model::Node & obj2) const
	{
		if(this->node.index == obj2.index)
			return true;
		else
			return false;
	}
float g_value;
};

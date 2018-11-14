#include "a_search.h"
#include <cmath>
#include <algorithm>


Search::Search(Model &model ):
    m_Model(model)
{
    /*Set start and end points*/
    end = m_Model.m_Nodes[m_Model.m_Ways[3].nodes[0]];
    
    Calculate_H_Value(end);
    start = m_Model.m_Nodes[m_Model.m_Ways[0].nodes[0]];

    m_Model.start_position = start;
    m_Model.end_position = end;

    //Call A* algorithm
    A_Star();


}

bool Search::A_Star(){

    /*
    TODO:
    2. Initialize a empty vector, for the open nodes
    x. Insert into the open list of nodes, the start point
    3. Initilize the g value.
    1. Calculate the h value for all the nodes.
    x. As long as there is nodes in the open list, keep looking at the nodes
    x. Call a function that give you the best next node to move 
    x. Add it to the path.
    x. Check if the current node is the goal, if it is then return true.
    x. If the current node is not the goal, then look for its neighbor.
    x. Push the nodes into the open list of nodes.
    x. Increment the g value.
    x. If you ran out of nodes without findind a path, then there is no path.
    */

    std::vector<Model::Node>open;
    open.emplace_back(start);
    Model::Node current_node = open.back();
    float g_value = 0.0;
    float stop = 11.0;
    while(open.size() > 0 && g_value< stop)
    // while (open.size() > 0)
    {
        current_node = Next_Node(open, g_value, current_node);
	std::cout<<"current_node: "<<current_node.index << " visited: " << current_node.visited << std::endl;
        m_Model.path.emplace_back(current_node);
        if(current_node.x == end.x && current_node.y == end.y ) 
        {
            std::cout<<"Hooray for you!"<<std::endl;
            return true;
        }
        
        std::vector<Model::Node> neighbors = Find_Neighbors(current_node);

        
        for (auto &node : neighbors)
        {
            //open.emplace_back(node);
	    std::vector<Model::Node>::iterator i = std::find_if(open.begin(),
    open.end(), 
    [&](const auto& node_in){ return node_in.index == node.index; } );
	    if (i == std::end(open) ){
                std::cout<<"here"<<std::endl;
	        open.emplace_back(node);
	    }
        }
	current_node.visited =true;
        g_value++;
        std::cout<<"current_node: "<<current_node.index << " visited: " << current_node.visited << std::endl;
        
        
    }
    
    std::cout<<"Didn't find it!"<<std::endl;

    
    return false;
}


float Search::Calculate_distance(Model::Node current_node, Model::Node other_node){
    return std::sqrt(std::pow((current_node.x - other_node.x),2)+ std::pow((current_node.y - other_node.y),2));
}

Model::Node Search::Next_Node(std::vector<Model::Node>&open, float gValue, Model::Node current_node){
    float lowest_fvalue = std::numeric_limits<float>::max();
    int next_node_pos;
    int counter = 0;
    int temp = 0;
    std::cout<<"+++Next node++++ "<<std::endl;
    for(Model::Node &node : open  ){
        counter++;
        float distance = Calculate_distance(current_node, node);
        float fvalue =  node.h_value + gValue + distance;
        //std::cout<<"node_id: "<<node.index<<" fvalue: "<<fvalue<<" distance: "<<distance<< " node.h_value: "<<node.h_value<<std::endl;
	
        if (!node.visited){
	    std::cout<<"node_index: "<<node.index<<"  visited: "<<node.visited<<std::endl;	
            if (fvalue<lowest_fvalue){

                lowest_fvalue = fvalue ;
                next_node_pos = node.index;
		
                temp = counter;
            }
            else
            {
                next_node_pos = next_node_pos;
                temp = temp;
            }
        }

    }

    
    if (open.size() == 1)
    {
        open.pop_back();
    }
    else
    {
        open.erase(open.begin()+temp-1);
    }
    m_Model.next_position = m_Model.m_Nodes[next_node_pos];

    //m_Model.m_Nodes[next_node_pos].visited = true;
std::cout<<"next_node_index: "<<next_node_pos << " visited: " << m_Model.m_Nodes[next_node_pos].visited << std::endl;
    std::cout<<"****************************************"<<std::endl;
	
    return m_Model.m_Nodes[next_node_pos];

}


void Search::Calculate_H_Value(Model::Node end) 
{
    float h_value;
    for(auto &node: m_Model.m_Nodes) {
        h_value = std::sqrt(std::pow((end.x - node.x),2)+ std::pow((end.y - node.y),2));
        node.h_value = h_value;     

    }
}


std::vector<Model::Node > Search::Find_Neighbors(Model::Node currentPosition)
{
    std::vector<Model::Node > neighbors;
    m_Model.neighbors = {};
    
    for(auto way_num : currentPosition.way_num)
    {
        auto way =m_Model.Ways()[way_num];

        if( way.type_highway == "primary" || way.type_highway == "secondary"
             || way.type_highway == "service" || way.type_highway == "residential"
             || way.type_highway == "tertiary" ){
            for(int i=0; i<m_Model.Ways()[way_num].id_nodes.size(); i++)
            {
                int node_index = m_Model.Ways()[way_num].nodes[i];
                std::string node_id = m_Model.Ways()[way_num].id_nodes[i];
                if (node_id != currentPosition.id)
                { 
                    if (!m_Model.m_Nodes[node_index].visited){
			//std::cout<<"next_node_pos: "<<node_index << " visited: " << m_Model.m_Nodes[node_index].visited << std::endl;
                        neighbors.emplace_back(m_Model.m_Nodes[node_index]);
                        m_Model.neighbors.emplace_back(node_index);
			//std::cout <<"next_node_pos: "<<node_index << "  new"<<std::endl;
                    }
                    //else{
                        //std::cout <<"next_node_pos: "<<node_index <<"  visited"<<std::endl;
                        
                    //}

                }
            }
            
                
                
            }
    }

    for (auto &node : neighbors)
        {
            std::cout <<"neighbors: "<<node.index <<"  visited: "<<node.visited<<std::endl;
        }
    //std::cout <<"+++++++++++++++++++++++++++"<<std::endl;
    
    return neighbors;  
}

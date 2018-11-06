#include "a_search.h"
#include <cmath>
#include <algorithm>


Search::Search(Model &model ):
    m_Model(model)
{
    
    int pos_Node_start = m_Model.m_Ways[0].nodes[0];
    m_Model.pos_Node_start = pos_Node_start;

    m_Model.start_position = m_Model.m_Nodes[pos_Node_start];


    int pos_Node_end = m_Model.m_Ways[1].nodes[0];
    m_Model.pos_Node_end = pos_Node_end;
    m_Model.end_position = m_Model.m_Nodes[pos_Node_end];
    

    start = m_Model.start_position;
    end = m_Model.end_position;

    A_Star();


}

bool Search::A_Star(){

    std::vector<int>open;
    open.emplace_back(m_Model.pos_Node_start);

    Calculate_H_Value(end);
    float g_value = 0.0;


    while (open.size() > 0)
    {
        Model::Node current_node = Next_Node(open, g_value);
        m_Model.path.emplace_back(current_node);
        if(current_node.x == end.x && current_node.y == end.y ) 
        {
            std::cout<<"Hooray for you!"<<std::endl;
            return true;
        }
        
        std::vector<int> neighbors = Find_Neighbors(current_node);
        for (auto node : neighbors)
        {
            open.emplace_back(node);
        }

        g_value++;
        std::string response;
        
    }
    
    std::cout<<"Didn't find it!"<<std::endl;

    
    return false;
}


Model::Node Search::Next_Node(std::vector<int>&neighbors, float gValue){
    float lowest_fvalue = std::numeric_limits<float>::max();
    int next_node_pos;
    int counter = 0;
    int temp = 0;
    for(int node : neighbors  ){
        counter++;
        float fvalue =  m_Model.m_Nodes[node].h_value + gValue ;
        // if (!m_Model.m_Nodes[node].visited){
            if (fvalue<lowest_fvalue){
                lowest_fvalue = fvalue ;
                next_node_pos = node;
                temp = counter;
            }
            else
            {
                next_node_pos = next_node_pos;
                temp = temp;
            }
        // }

    }
    if (neighbors.size() == 1)
    {
        neighbors.pop_back();
    }
    else
    {
        neighbors.erase(neighbors.begin()+temp-1);
    }
    m_Model.next_position = m_Model.m_Nodes[next_node_pos];

    m_Model.m_Nodes[next_node_pos].visited = true;
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

std::vector<int> Search::Find_Neighbors(Model::Node currentPosition)
{
    std::vector<int> neighbors;
    
    for(auto way_num : currentPosition.way_num)
    {
        auto way =m_Model.Ways()[way_num];

        if( way.type_highway == "primary" || way.type_highway == "secondary"
             || way.type_highway == "service" || way.type_highway == "residential" ){
            for(int i=0; i<m_Model.Ways()[way_num].id_nodes.size(); i++)
            {
                int node_index = m_Model.Ways()[way_num].nodes[i];
                std::string node_id = m_Model.Ways()[way_num].id_nodes[i];
                if (node_id != currentPosition.id)
                { 
                    if (!m_Model.m_Nodes[node_index].visited){
                        neighbors.emplace_back(node_index);
                    }
                }
            }
            
                
                
            }
    }
    return neighbors;  
}
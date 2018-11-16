#include "a_search.h"
#include <cmath>
#include <algorithm>
#include <set>


Search::Search(Model &model ):
    m_Model(model)
{
    /*Set start and end points*/
    //end = m_Model.m_Nodes[m_Model.m_Ways[3].nodes[0]];
    end = m_Model.m_Nodes[1113];
    Calculate_H_Value(end);
    start = m_Model.m_Nodes[m_Model.m_Ways[0].nodes[0]];

    m_Model.start_position = start;
    m_Model.end_position = end;
    std::vector<OpenNode> openlist;

    //Call A* algorithm
    A_Star(openlist);
    /*std::cout<<"openlist.size: " << openlist.size()<<"\n";
    std::vector<int> parents;
    
    for (auto node : openlist){
        
        //std::cout<<"node: " << node.node.index<<"\n";
        for(auto parent: node.parents){
            std::cout<<"node: " << node.node.index<<"  parent: " << parent.index<<"\n";
            parents.emplace_back(parent.index);
        }
        //m_Model.path.emplace_back(current_node);
    }
    
    for(auto parent: parents){
            std::cout<<"parent: " << parent<<"\n";
            m_Model.path.emplace_back(m_Model.m_Nodes[parent]);
    }*/

   /*TEST ZONE
    std::vector<OpenNode>openlist;
    OpenNode current;
    current.node = start;
    current.parents = {};
    openlist.emplace_back(current);
    float gValue = 0.0;

    OpenNode next = Next_Node(openlist,  gValue, current);
    std::cout<<"next: "<< next.node.index << "  parent_size: "<< next.parents.size()<<"\n";
    */

}

bool Search::A_Star(std::vector<OpenNode> &openlist){

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

    //std::vector<Model::Node>open;
    

    //open.emplace_back(start);
    //Model::Node current_node = open.back();
    
    float g_value = 0.0;
    //57.0
    float stop = 25.0;
    OpenNode open_node;
    
    open_node.node = start;
    
    openlist.emplace_back(open_node);
    OpenNode current_node = openlist.back();

    /*Expand nodes until you reach the goal. Use heuristic to prioritize what node to open first*/
    //while(openlist.size() > 0 && g_value< stop)
    while (openlist.size() > 0)
    {   //Select the best node to explore next. 
        //std::cout<<"openlist.size(): "<< openlist.size() <<std::endl;
        //current_node = Next_Node(open, g_value, current_node);
    
        current_node = Next_Node(openlist,  g_value, current_node);
         
        //m_Model.path.emplace_back(current_node);
      
        //Check if the node selected is the goal.   
        if(current_node.node.x == end.x && current_node.node.y == end.y ) 
        {
            std::cout<<"Hooray for you!"<<std::endl;
            /*float distance = 0.0;
            Model::Node current_node = m_Model.path.front();
                for (auto node: m_Model.path ){
                    distance += Calculate_distance(current_node, node);
            current_node = node;
            }
            std::cout<<"Distance: "<< distance <<std::endl;*/
            std::vector<Model::Node> path_found = current_node.parents;
            path_found.emplace_back(current_node.node);
            
            for (auto node : path_found){
                    std::cout<<"node: " << node.index<<"\n";
            }
            return true;
        }
        
        //Expand the current node (add all unvisited neighbors to the open list)  
        std::vector<Model::Node> neighbors = Find_Neighbors(current_node.node);
        //std::cout<<"++++neighbors++++"<<std::endl;
        
        //Keep track of the path used to reach the new nodes going into open list.
        std::vector<Model::Node> neighbor_parents = current_node.parents;
        neighbor_parents.emplace_back(current_node.node);

        for (auto neighbor : neighbors)
        {
             //buid an OpenNode object
             open_node.node = neighbor;
             open_node.parents = neighbor_parents;
             
             //Add the neighbor to the open list.
             openlist.emplace_back(open_node);
        }
              
    }  //openlist while loop
    
    //std::cout<<"\n\n";
    //std::cout<<"Openlist size:"<< openlist.size()<<"\n";
    //std::cout<<"g_value: "<< g_value<<"\n\n";
    std::cout<<"Didn't find it!"<<std::endl;

    
    return false;
}

OpenNode Search::Next_Node(std::vector<OpenNode>&openlist, float gValue, OpenNode current_node){
    float lowest_fvalue = std::numeric_limits<float>::max();
    
    OpenNode lowest_node;
    int next_node_pos;
    int counter = 0;
    int temp = 0;
    for(OpenNode &o_node : openlist  ){
        
        Model::Node node = o_node.node;
        counter++;
        float distance = Calculate_distance(current_node.node, node);
        float fvalue =  node.h_value + gValue+ distance;
        //std::cout<<"node_index: "<<node.index<<" fvalue: "<<fvalue<<" distance: "<<distance<< " node.h_value: "<<node.h_value<<std::endl;
        if (!node.visited){
            if (fvalue<lowest_fvalue){
                lowest_fvalue = fvalue ;
                next_node_pos = node.index;
                temp = counter;
                lowest_node = o_node;
            }
            else
            {
                next_node_pos = next_node_pos;
                lowest_node = lowest_node;
                temp = temp;
            }
        }

    }

    //std::cout<<"next_node_pos: "<<next_node_pos << " lowest_fvalue: " << lowest_fvalue<< " id: " << m_Model.m_Nodes[next_node_pos].id << std::endl;
   //std::cout<<"****************************************"<<std::endl;
    if (openlist.size() == 1)
    {
        openlist.pop_back();
    }
    else
    {
        openlist.erase(openlist.begin()+temp-1);
    }
    m_Model.next_position = m_Model.m_Nodes[next_node_pos];

    m_Model.m_Nodes[next_node_pos].visited = true;
    
    return lowest_node;

}



float Search::Calculate_distance(Model::Node current_node, Model::Node other_node){
    return std::sqrt(std::pow((current_node.x - other_node.x),2)+ std::pow((current_node.y - other_node.y),2));
}

Model::Node Search::Next_Node(std::vector<Model::Node>&open, float gValue, Model::Node current_node){
    float lowest_fvalue = std::numeric_limits<float>::max();
    int next_node_pos;
    int counter = 0;
    int temp = 0;
    for(Model::Node &node : open  ){
        counter++;
        float distance = Calculate_distance(current_node, node);
        float fvalue =  node.h_value + gValue+ distance;
        //std::cout<<"node_index: "<<node.index<<" fvalue: "<<fvalue<<" distance: "<<distance<< " node.h_value: "<<node.h_value<<std::endl;
        if (!node.visited){
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

    //std::cout<<"next_node_pos: "<<next_node_pos << " lowest_fvalue: " << lowest_fvalue<< " id: " << m_Model.m_Nodes[next_node_pos].id << std::endl;
   //std::cout<<"****************************************"<<std::endl;
    if (open.size() == 1)
    {
        open.pop_back();
    }
    else
    {
        open.erase(open.begin()+temp-1);
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


std::vector<Model::Node > Search::Find_Neighbors(Model::Node currentPosition)
{
    std::vector<Model::Node > neighbors={};
    m_Model.neighbors = {};
    
    for(auto way_num : currentPosition.way_num)
    {
        auto way =m_Model.Ways()[way_num];

        if( true){
            float lowest_distance = 10000000.0 ;
            int lowest_index = -1;
            //lowest_node = ; 
            for(int i=0; i<m_Model.Ways()[way_num].id_nodes.size(); i++)
            {
                int node_index = m_Model.Ways()[way_num].nodes[i];
                std::string node_id = m_Model.Ways()[way_num].id_nodes[i];
                

                if (node_id != currentPosition.id)
                { 
                    if (!m_Model.m_Nodes[node_index].visited){
                    float distance = Calculate_distance(currentPosition, m_Model.m_Nodes[node_index]);
                        if (distance < lowest_distance){
                            lowest_distance = distance;
                            lowest_index = node_index; 
                        }

                    }
                }
            }
            if(lowest_index >= 0){
                neighbors.emplace_back(m_Model.m_Nodes[lowest_index]);
                //std::cout<< "new_neighbor_index: "<<lowest_index<< "  distance: "<<lowest_distance<<"  way_num: "<<way_num<< "\n";
                m_Model.neighbors.emplace_back(lowest_index);
                
            }
        }
    }
    //std::cout<< "neighbors size: "<<m_Model.neighbors.size() <<"\n";
    //std::cout<< "**********************"<<std::endl;
    //std::cout << "number of ways = " << currentPosition.way_num.size() << "\n";
    //std::cout << "number of neighbors = " << neighbors.size() << "\n\n";
    return neighbors;  
}

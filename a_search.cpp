#include "a_search.h"
#include <cmath>
#include <algorithm>
#include <set>


Search::Search(Model &model ):
    m_Model(model)
{
    /*Set start and end points*/
    end = m_Model.Nodes()[m_Model.Ways()[5].nodes[0]];
    Calculate_H_Value(end);
    start = m_Model.Nodes()[m_Model.Ways()[0].nodes[0]];

    m_Model.start_position = start;
    m_Model.end_position = end;
    std::vector<OpenNode> openlist;

    //Call A* algorithm
    m_Model.path = A_Star(openlist);

}

std::vector<Model::Node> Search::A_Star(std::vector<OpenNode> &openlist){

    /*
    TODO:
    1. Create a new object of type OpenNode.
    2. Assign the node member to the start node.
    3. Initialize an empty vector, for the open nodes.
    4. Insert into the open list, the OpenNode.
    5. Initilize the g value.
    6. Calculate the h value for all the nodes.
    7. As long as there is nodes in the open list, keep looking at the nodes
    8. Call a function that give you the best next node to move based on the heuristic 
    9. Check if the selected node is the goal, if it is then return the path found adding the nodes in the openlist and the current node.
    10. If the current node is not the goal, then look for its neighbor.
    11. Concatenate the parents seen to the current. 
    12. Push the nodes into the open list of nodes.
    13. Increment the g value.
    14. If you ran out of nodes without finding a path, then there is no path.
    */
    
    float g_value = 0.0;


    OpenNode open_node;
    open_node.node = start;
    
    openlist.emplace_back(open_node);
    OpenNode current_node = openlist.back();

    /*Expand nodes until you reach the goal. Use heuristic to prioritize what node to open first*/
    //while(openlist.size() > 0 && g_value< stop)
    while (openlist.size() > 0)
    {   
        //Select the best node to explore next. 
        current_node = Next_Node(openlist,  g_value, current_node);
      
        //Check if the node selected is the goal.   
        if(current_node.node.x == end.x && current_node.node.y == end.y ) 
        {
            std::cout<<"Hooray for you!"<<std::endl;
            
            //Build the path with all the parents and the current node.
            std::vector<Model::Node> path_found = current_node.parents;
            path_found.emplace_back(current_node.node);
            
            /*float distance = 0.0;
            Model::Node curr = path_found.front();
            for (auto node : path_found){
                distance += Calculate_distance(node, curr);
                curr = node;
                
            }
            std::cout<<"distance: " << distance <<"\n";*/
            return path_found;
        }
        
        //Expand the current node (add all unvisited neighbors to the open list)  
        std::vector<Model::Node> neighbors = Find_Neighbors(current_node.node);
        
        //Keep track of the path used to reach the new nodes going into open list.
        std::vector<Model::Node> neighbor_parents = current_node.parents;
        neighbor_parents.emplace_back(current_node.node);

        for (auto neighbor : neighbors)
        {
             //Buid an OpenNode object
             open_node.node = neighbor;
             open_node.parents = neighbor_parents;
             
             //Add the neighbor to the open list.
             openlist.emplace_back(open_node);
        }
              
    }  //openlist while loop
    
    std::cout<<"Didn't find it!"<<std::endl;
    
    return {};
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

    if (openlist.size() == 1)
    {
        openlist.pop_back();
    }
    else
    {
        openlist.erase(openlist.begin()+temp-1);
    }
    m_Model.next_position = m_Model.Nodes()[next_node_pos];

    m_Model.Nodes()[next_node_pos].visited = true;
    
    return lowest_node;

}



float Search::Calculate_distance(Model::Node current_node, Model::Node other_node){
    return std::sqrt(std::pow((current_node.x - other_node.x),2)+ std::pow((current_node.y - other_node.y),2));
}




void Search::Calculate_H_Value(Model::Node end) 
{
    float h_value;
    for(auto &node: m_Model.Nodes()) {
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
            float lowest_distance = std::numeric_limits<float>::max() ;
            int lowest_index = -1;
            for(int i=0; i<m_Model.Ways()[way_num].id_nodes.size(); i++)
            {
                int node_index = m_Model.Ways()[way_num].nodes[i];
                std::string node_id = m_Model.Ways()[way_num].id_nodes[i];
                

                if (node_id != currentPosition.id)
                { 
                    if (!m_Model.Nodes()[node_index].visited){
                    float distance = Calculate_distance(currentPosition, m_Model.Nodes()[node_index]);
                        if (distance < lowest_distance){
                            lowest_distance = distance;
                            lowest_index = node_index; 
                        }

                    }
                }
            }
            if(lowest_index >= 0){
                neighbors.emplace_back(m_Model.Nodes()[lowest_index]);
                m_Model.neighbors.emplace_back(lowest_index);
                
            }
        }
    }
    return neighbors;  
}

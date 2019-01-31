#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include "search_model.h"


class Search {
 public:
    Search(SearchModel &model): m_Model(model) {}
    float GetDistance() const {return distance;}
    void AddNeighbors(SearchModel::Node * current_node);
    SearchModel::Node *NextNode();
    void AStar();
    std::vector<SearchModel::Node> CreatePathFound(SearchModel::Node *);

 private:
    SearchModel &m_Model;
    float distance;
    std::vector<SearchModel::Node *> open_list;
};

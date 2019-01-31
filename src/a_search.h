#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include "search_model.h"


class Search {
  public:
    Search(SearchModel &model): m_Model(model) {}
    float GetDistance() const {return distance;}
    void AStar();

  private:
    void AddNeighbors(SearchModel::Node *current_node);
    std::vector<SearchModel::Node> CreatePathFound(SearchModel::Node *);
    SearchModel::Node *NextNode();
    SearchModel &m_Model;
    float distance;
    std::vector<SearchModel::Node*> open_list;
};

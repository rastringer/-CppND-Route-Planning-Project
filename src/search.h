#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include "search_model.h"


class Search {
  public:
    Search(SearchModel &model, float start_x, float start_y, float end_x, float end_y);
    float GetDistance() const {return distance;}
    void AStar();

  private:
    void AddNeighbors(SearchModel::Node *current_node);
    void CalculateHValues();
    std::vector<SearchModel::Node> ConstructFinalPath(SearchModel::Node *);
    SearchModel::Node *NextNode();
    SearchModel &m_Model;
    SearchModel::Node *start_node;
    SearchModel::Node *end_node;
    float distance;
    std::vector<SearchModel::Node*> open_list;
};

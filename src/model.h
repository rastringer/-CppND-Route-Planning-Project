#pragma once

#include <cstddef>
#include <limits>
#include <cmath>
#include <string>
#include <unordered_map>
#include <vector>

class Model
{
public:
    struct Node {
        std::string id = "";
        std::vector<int> way_nums;
        int index;
        int parent_index = -1;
        float x = 0.f;
        float y = 0.f;
        float h_value = std::numeric_limits<float>::max();
        float g_value = 0.0;
        bool visited = false;

        float distance(Node other) {
            return std::sqrt(std::pow((x - other.x),2) + std::pow((y - other.y),2));
        }
    };
    
    struct Way {
        std::vector<int> nodes;
        std::vector<std::string> id_nodes;
        std::string type_highway;
    };
    
    struct Road {
        enum Type { Invalid, Unclassified, Service, Residential,
            Tertiary, Secondary, Primary, Trunk, Motorway, Footway };
        int way;
        Type type;
    };
    
    struct Railway {
        int way;
    };    
    
    struct Multipolygon {
        std::vector<int> outer;
        std::vector<int> inner;
    };
    
    struct Building : Multipolygon {};
    
    struct Leisure : Multipolygon {};
    
    struct Water : Multipolygon {};
    
    struct Landuse : Multipolygon {
        enum Type { Invalid, Commercial, Construction, Grass, Forest, Industrial, Railway, Residential };
        Type type;
    };
    
    Model( const std::vector<std::byte> &xml );
    
    auto MetricScale() const noexcept { return m_MetricScale; }    
    auto &Nodes() { return m_Nodes; }
    auto &Ways() const noexcept { return m_Ways; }
    auto &Roads() const noexcept { return m_Roads; }
    auto &Buildings() const noexcept { return m_Buildings; }
    auto &Leisures() const noexcept { return m_Leisures; }
    auto &Waters() const noexcept { return m_Waters; }
    auto &Landuses() const noexcept { return m_Landuses; }
    auto &Railways() const noexcept { return m_Railways; }
    Node &FindClosestNode(Node);
    
    double min_x;
    double min_y;

    Node start_node;
    Node end_node;
    std::vector<Node> path;
    std::vector<int> parents;
    
private:
    void AdjustCoordinates();
    void BuildRings( Multipolygon &mp );
    void LoadData(const std::vector<std::byte> &xml);
    void Get_shared_nodes();
    
    std::vector<Node> m_Nodes;
    std::vector<Way> m_Ways;
    std::vector<Road> m_Roads;
    std::vector<Railway> m_Railways;
    std::vector<Building> m_Buildings;
    std::vector<Leisure> m_Leisures;
    std::vector<Water> m_Waters;
    std::vector<Landuse> m_Landuses;
    
    double m_MinLat = 0.;
    double m_MaxLat = 0.;
    double m_MinLon = 0.;
    double m_MaxLon = 0.;
    double m_MetricScale = 1.f;

};

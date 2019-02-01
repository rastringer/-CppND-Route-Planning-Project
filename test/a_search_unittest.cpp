#include "gtest/gtest.h"
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>
#include "../src/route_model.h"
#include "../src/route_planner.h"

// TODO: factor this out of main.cpp so we can re-use
static std::optional<std::vector<std::byte>> ReadFile(const std::string &path)
{   
    std::ifstream is{path, std::ios::binary | std::ios::ate};
    if( !is )
        return std::nullopt;
    
    auto size = is.tellg();
    std::vector<std::byte> contents(size);    
    
    is.seekg(0);
    is.read((char*)contents.data(), size);

    if( contents.empty() )
        return std::nullopt;
    return std::move(contents);
}

// test that search returns the expected distance 0.43332419
TEST(VerifyAStar, Distance) {
    std::vector<std::byte> osm_data;
    
    std::string osm_data_file = "../map.osm";
 
    if( osm_data.empty() && !osm_data_file.empty() ) {
        std::cout << "Reading OpenStreetMap data from the following file: " <<  osm_data_file << std::endl;
        auto data = ReadFile(osm_data_file);
        if( !data )
            std::cout << "Failed to read." << std::endl;
        else
            osm_data = std::move(*data);
    }
    
    RouteModel model{osm_data};  
    RoutePlanner route_planner{model, 0, 0, 50, 50};
    EXPECT_FLOAT_EQ(0.43332419, route_planner.GetDistance());
}
#include "gtest/gtest.h"
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>
#include "../src/model.h"
#include "../src/a_search.h"

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
    
    Model model{osm_data};  
    Search search{model};
    EXPECT_FLOAT_EQ(0.43332419, search.GetDistance());
}
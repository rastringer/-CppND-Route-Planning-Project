#include <optional>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <io2d.h>
#include "model.h"
#include "render.h"
#include "a_search.h"

using namespace std::experimental;

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

int main(int argc, const char **argv)
{    
    std::string osm_data_file = "";
    if( argc > 1 ) {
        for( int i = 1; i < argc; ++i )
            if( std::string_view{argv[i]} == "-f" && ++i < argc )
                osm_data_file = argv[i];
    }
    else {
        std::cout << "Usage: [executable] [-f filename.osm]" << std::endl;    
    }
    
    std::vector<std::byte> osm_data;
 
    if( osm_data.empty() && !osm_data_file.empty() ) {
        std::cout << "Reading OpenStreetMap data from the following file: " <<  osm_data_file << std::endl;
        auto data = ReadFile(osm_data_file);
        if( !data )
            std::cout << "Failed to read." << std::endl;
        else
            osm_data = std::move(*data);
    }
    
    // Build Model.
    Model model{osm_data};
    std::cout << "The map coordinates begin at (0,0) in the lower left, and the max value for x and y is: " << model.MetricScale() << "\n";
    std::cout << "Enter a start x: ";
    std::cin >> model.start_node.x;
    std::cout << "Enter a start y: ";
    std::cin >> model.start_node.y;
    std::cout << "Enter an end x: ";
    std::cin >> model.end_node.x;
    std::cout << "Enter an end y: ";
    std::cin >> model.end_node.y;
    // Scale node x and y values and find closest matching nodes in Model.
    model.start_node.x /= model.MetricScale();
    model.start_node.y /= model.MetricScale();
    model.end_node.x /= model.MetricScale();
    model.end_node.y /= model.MetricScale();
    model.start_node = model.FindClosestNode(model.start_node);
    model.end_node = model.FindClosestNode(model.end_node);

    // Perform search and render results.
    Search search{model};
    Render render{model};

    auto display = io2d::output_surface{400, 400, io2d::format::argb32, io2d::scaling::none, io2d::refresh_style::fixed, 30};
    display.size_change_callback([](io2d::output_surface& surface){
        surface.dimensions(surface.display_dimensions());
    });
    display.draw_callback([&](io2d::output_surface& surface){
        render.Display(surface);
    });
    display.begin_show();
}

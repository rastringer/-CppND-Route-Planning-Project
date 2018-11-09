cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 17)

# Use the CMakeLists.txt's parent-directory-name for the project's id/name
get_filename_component(PROJECT_ID ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" PROJECT_ID ${PROJECT_ID})
project(${PROJECT_ID})


find_package(io2d REQUIRED)
find_package(Cairo) 
find_package(GraphicsMagick) 

add_subdirectory(external/pugixml)

add_executable(${PROJECT_ID}
    main.cpp
    model.cpp
    model.h
    render.cpp
    render.h
    a_search.cpp
    a_search.h
)

target_link_libraries(${PROJECT_ID}
    PRIVATE io2d::io2d
    PUBLIC pugixml
    
)


if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    target_link_libraries(${PROJECT_ID} PUBLIC pthread)
endif()

if(MSVC)
	target_compile_options(${PROJECT_ID} PUBLIC /D_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING /wd4459)
endif()

cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_ID maps)
project(${PROJECT_ID})

# Setup default installation subpaths, if they were not defined outside
if(NOT DEFINED CMAKE_INSTALL_BINDIR)
    set(CMAKE_INSTALL_BINDIR bin)
endif()
if(NOT DEFINED CMAKE_INSTALL_INCLUDEDIR)
    set(CMAKE_INSTALL_INCLUDEDIR include/io2d)
endif()
if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
    set(CMAKE_INSTALL_LIBDIR lib)
endif()



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
    ${Cairo_LIBRARIES}
    ${ImageMagick_LIBRARIES}
    
)
target_include_directories(${PROJECT_ID} SYSTEM
    PRIVATE ${Boost_INCLUDE_DIRS}
    PRIVATE ${OPENSSL_INCLUDE_DIR}
)

include_directories(${Cairo_INCLUDE_DIRS}) 
include_directories(${ImageMagick_INCLUDE_DIRS}) 

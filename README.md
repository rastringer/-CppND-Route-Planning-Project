In this exercise, complete `AStarSearch()` in `route_planner.cpp` using the `NextNode`, `ConstructFinalPath`, and `AddNeighbors` methods you have written previously.

### Dependencies in Linux
1. g++7: sudo apt-get install gcc-7 g++-7
2. CMake 3.8 or higher:  (https://cmake.org/install/) 
3. Cairo: sudo apt-get install libcairo2-dev
4. graphicsmagick: sudo apt-get install libgraphicsmagick1-dev
5. libpng: sudo apt-get install libpng-dev

### How to run with CMake

```
export CXX=g++-7
git clone --recurse-submodules https://github.com/udacity/CppND-Route-Planning-Solution.git
cd CppND-Route-Planning-Solution
mkdir build 
cd build
cmake ..
make
../bin/test
../bin/CppND-Route-Planning-Solution -f ../map.osm
````

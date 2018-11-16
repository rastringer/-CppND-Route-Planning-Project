### Dependencies in Linux
1. g++7: sudo apt-get install gcc-7 g++-7
2. CMake 3.8 or higher:  (https://cmake.org/install/) 
3. Cairo: sudo apt-get install libcairo2-dev
4. graphicsmagick: sudo apt-get install libgraphicsmagick1-dev
5. libpng: sudo apt-get install libpng-dev
6. io2d

### How to run with CMake

```
export CXX=g++-7
git clone --recurse-submodules https://github.com/udacity/CppND-Route-Planning.git
cd CppND-Route-Planning
mkdir build 
cd build
cmake ..
make
./maps -f ../maps.osm
````

# CMakeList.txt : CMake project for PointsCloud, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.5)


project ("unitttest")
#允许GDB调试
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG   "$ENV{CXXFLAGS} -o0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -o3 -Wall")
 



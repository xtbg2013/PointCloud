# CMakeList.txt : CMake project for PointsCloud, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.5)


project ("test")

set (EXECUTABLE_OUTPUT_PATH ../)

aux_source_directory(. SOURCE_PATH)

add_executable(${PROJECT_NAME} ${SOURCE_PATH})
target_link_libraries(${PROJECT_NAME} PUBLIC gtest pthread)
 
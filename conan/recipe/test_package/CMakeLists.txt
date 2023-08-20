cmake_minimum_required(VERSION 3.20)
project(test_package CXX)

find_package(yabil REQUIRED CONFIG)
find_package(TBB REQUIRED CONFIG)

add_executable(${PROJECT_NAME} test_package.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE yabil::bigint yabil::crypto yabil::utils yabil::math)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED TRUE
)

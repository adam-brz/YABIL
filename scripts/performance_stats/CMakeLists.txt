cmake_minimum_required(VERSION 3.23)
project(yabil_performance_stats LANGUAGES CXX)

find_package(yabil REQUIRED)
find_package(gmp REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE yabil::bigint yabil::crypto gmp::gmp)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED TRUE
)

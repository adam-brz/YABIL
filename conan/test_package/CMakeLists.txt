cmake_minimum_required(VERSION 3.23)
project(PackageTest CXX)

find_package(yabil REQUIRED CONFIG)

add_executable(${PROJECT_NAME} example.cpp)
target_link_libraries(${PROJECT_NAME} yabil::bigint yabil::crypto)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED TRUE
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

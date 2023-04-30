cmake_minimum_required(VERSION 3.23)
project(yabil_benchmarks)

find_package(benchmark REQUIRED CONFIG)
find_package(gmp REQUIRED CONFIG)
find_package(yabil REQUIRED CONFIG)

set(SOURCES
    src/addition.cpp
    src/multiplication.cpp
    src/utils.cpp
    src/utils.h
)

add_executable(${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED TRUE
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    yabil::bigint
    gmp::gmp
    benchmark::benchmark
    benchmark::benchmark_main
)

project(parallel C CXX)

set(SOURCES
    src/FunctionWrapper.cpp
    src/Parallel.cpp
    src/ParallelImpl.h
    src/ParallelStdImpl.cpp
    src/ParallelTBBImpl.cpp
    src/ThreadPool.cpp
    src/ThreadPoolSingleton.cpp
)

set(HEADERS
    include/yabil/parallel/FunctionWrapper.h
    include/yabil/parallel/Parallel.h
    include/yabil/parallel/ThreadPool.h
    include/yabil/parallel/ThreadPoolSingleton.h
)

set(TESTS
    test/FunctionWrapper_tests.cpp
    test/ParallelOperations_tests.cpp
    test/ThreadPoolSingleton_tests.cpp
    test/ThreadPool_tests.cpp
)

add_library(${PROJECT_NAME})

if(YABIL_ENABLE_TBB)
    list(APPEND SOURCES src/ParallelTBBImpl.cpp)
else()
    list(APPEND SOURCES src/ParallelStdImpl.cpp)
endif()

if(YABIL_ENABLE_TBB)
    target_link_libraries(${PROJECT_NAME} PRIVATE TBB::tbb)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC bigint PRIVATE utils)

target_sources(${PROJECT_NAME}
    PRIVATE ${SOURCES}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES ${HEADERS}
)

set_common_properties(${PROJECT_NAME})
add_test_target(${PROJECT_NAME} ${TESTS})

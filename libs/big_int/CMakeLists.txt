project(big_int C CXX)

set(SOURCES
    src/test.cpp
)

set(HEADERS
)

set(TESTS
    test/empty.cpp
)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
set_common_properties(${PROJECT_NAME})
add_test_target(${PROJECT_NAME} ${TESTS})

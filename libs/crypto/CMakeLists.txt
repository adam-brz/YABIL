project(crypto C CXX)

set(SOURCES
    src/main.cpp
)

set(HEADERS
)

set(TESTS
)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
set_common_properties(${PROJECT_NAME})
add_test_target(${PROJECT_NAME} ${TESTS})

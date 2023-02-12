project(crypto C CXX)

set(SOURCES
    src/crypto.cpp
)

set(HEADERS
    include/yabil/crypto/crypto.h
)

set(TESTS
)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
set_common_properties(${PROJECT_NAME})
add_test_target(${PROJECT_NAME} ${TESTS})

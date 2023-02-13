project(crypto C CXX)

set(SOURCES
    src/crypto.cpp
)

set(HEADERS
    include/yabil/crypto/crypto.h
)

set(TESTS
)

add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE ${SOURCES}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES ${HEADERS}
)

set_common_properties(${PROJECT_NAME})
add_test_target(${PROJECT_NAME} ${TESTS})

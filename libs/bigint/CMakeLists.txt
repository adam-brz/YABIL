project(bigint C CXX)

set(SOURCES
    src/test.cpp
)

set(HEADERS
    include/yabil/bigint/test.h
)

set(TESTS
    test/empty.cpp
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

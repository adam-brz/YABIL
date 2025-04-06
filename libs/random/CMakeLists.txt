project(random C CXX)

set(SOURCES
    src/RandomEngine.cpp
    src/Primes.h
)

set(HEADERS
    include/yabil/random/RandomEngine.h
)

set(TESTS
    test/Random_tests.cpp
)

add_library(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PUBLIC bigint PRIVATE math)

target_sources(${PROJECT_NAME}
    PRIVATE ${SOURCES}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES ${HEADERS}
)

set_common_properties(${PROJECT_NAME})
add_test_target(${PROJECT_NAME} ${TESTS})

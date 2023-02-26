project(bigint C CXX)

set(SOURCES
    src/BigInt.cpp
    src/SafeOperators.h
)

set(HEADERS
    include/yabil/bigint/BigInt.h
)

set(TESTS
    test/BigIntConstructor_tests.cpp
    test/BigIntConversion_tests.cpp
    test/BigIntOperator_tests.cpp
)

find_package(safeint REQUIRED)

add_library(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE safeint::safeint)

target_sources(${PROJECT_NAME}
    PRIVATE ${SOURCES}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES ${HEADERS}
)

set_common_properties(${PROJECT_NAME})
add_test_target(${PROJECT_NAME} ${TESTS})

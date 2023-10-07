project(math C CXX)

set(SOURCES
    src/Math.cpp
)

set(HEADERS
    include/yabil/math/Math.h
)

set(TESTS
    test/MathFactorial_tests.cpp
    test/MathLog_tests.cpp
    test/MathModInverse_tests.cpp
    test/MathPowOperator_tests.cpp
    test/MathGCD_tests.cpp
    test/MathSqrt_tests.cpp
    test/MathRoot_tests.cpp
)

add_library(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PUBLIC bigint visibility)

target_sources(${PROJECT_NAME}
    PRIVATE ${SOURCES}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES ${HEADERS}
)

set_common_properties(${PROJECT_NAME})
add_test_target(${PROJECT_NAME} ${TESTS})

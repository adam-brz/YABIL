project(compile_time C CXX)

set(HEADERS
    include/yabil/compile_time/ConstBigInt.h
    include/yabil/compile_time/Literals.h
    include/yabil/compile_time/Math.h
    include/yabil/compile_time/impl/ArithmeticOperators.h
    include/yabil/compile_time/impl/BitwiseOperators.h
    include/yabil/compile_time/impl/RelationOperators.h
    include/yabil/compile_time/impl/StringConversion.h
    include/yabil/compile_time/impl/Utils.h
)

set(TESTS
    test/Addition_tests.cpp
    test/Comparison_tests.cpp
    test/Constructor_tests.cpp
    test/Conversion_tests.cpp
    test/Literals_tests.cpp
    test/Multiplication_tests.cpp
    test/ShiftOperators_tests.cpp
)

add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE bigint)

target_sources(${PROJECT_NAME}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES ${HEADERS}
)

set_common_properties(${PROJECT_NAME})
add_test_target(${PROJECT_NAME} ${TESTS})

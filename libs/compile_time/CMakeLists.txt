project(compile_time C CXX)

set(HEADERS
    include/yabil/compile_time/BigIntData.h
    include/yabil/compile_time/ConstBigInt.h
    include/yabil/compile_time/Literals.h
    include/yabil/compile_time/Math.h

    include/yabil/compile_time/operators/ArithmeticOperators.h
    include/yabil/compile_time/operators/BitwiseOperators.h
    include/yabil/compile_time/operators/RelationOperators.h

    include/yabil/compile_time/impl/ArithmeticOperatorsImpl.h
    include/yabil/compile_time/impl/BitwiseOperatorsImpl.h
    include/yabil/compile_time/impl/MakeConstBigInt.h
    include/yabil/compile_time/impl/RelationOperatorsImpl.h
    include/yabil/compile_time/impl/StringConversion.h
    include/yabil/compile_time/impl/Utils.h
)

set(TESTS
    test/Addition_tests.cpp
    test/Comparison_tests.cpp
    test/Constructor_tests.cpp
    test/Conversion_tests.cpp
    test/Division_tests.cpp
    test/Literals_tests.cpp
    test/Multiplication_tests.cpp
    test/RelationOperators_tests.cpp
    test/ShiftOperators_tests.cpp
    test/Subtraction_tests.cpp
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

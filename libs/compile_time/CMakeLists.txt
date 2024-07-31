project(compile_time C CXX)

set(HEADERS
    include/yabil/compile_time/ConstBigInt.h
    include/yabil/compile_time/Literals.h
    include/yabil/compile_time/Math.h

    include/yabil/compile_time/detail/BigIntData.h
    include/yabil/compile_time/detail/ConstBigInt.h
    include/yabil/compile_time/detail/operators/ArithmeticOperators.h
    include/yabil/compile_time/detail/operators/BitwiseOperators.h
    include/yabil/compile_time/detail/operators/RelationOperators.h

    include/yabil/compile_time/impl/ConstBigIntImpl.h
    include/yabil/compile_time/impl/MakeConstBigInt.h
    include/yabil/compile_time/impl/MathImpl.h
    include/yabil/compile_time/impl/operators/ArithmeticOperatorsImpl.h
    include/yabil/compile_time/impl/operators/BitwiseOperatorsImpl.h
    include/yabil/compile_time/impl/operators/DivisionImpl.h
    include/yabil/compile_time/impl/operators/RelationOperatorsImpl.h
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

set(GENERATED_TESTS
    test/generated/AdditionGeneratedTests.cpp
    test/generated/BitOperationsGeneratedTests.cpp
    test/generated/DivisionGeneratedTests.cpp
    test/generated/MultiplicationGeneratedTests.cpp
    test/generated/RelationGeneratedTests.cpp
    test/generated/SubtractionGeneratedTests.cpp
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
add_test_target(${PROJECT_NAME}_generated ${GENERATED_TESTS})

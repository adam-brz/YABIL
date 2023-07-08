project(bigint C CXX)

set(SOURCES
    src/Arithmetic.cpp
    src/Arithmetic.h
    src/AVX2Utils.cpp
    src/AVX2Utils.h
    src/BigInt.cpp
    src/BigIntArithmeticOperators.cpp
    src/BigIntBinaryOperators.cpp
    src/BigIntGlobalConfig.cpp
    src/BigIntReleationOperators.cpp
    src/StringConversionUtils.cpp
    src/StringConversionUtils.h
    src/Thresholds.h
    src/TypeUtils.h
    src/parallel/Parallel.cpp
    src/parallel/ParallelImpl.h
)

if(YABIL_ENABLE_TBB)
    list(APPEND SOURCES src/parallel/ParallelTBBImpl.cpp)
else()
    list(APPEND SOURCES src/parallel/ParallelStdImpl.cpp)
endif()

set(HEADERS
    include/yabil/bigint/BigInt.h
    include/yabil/bigint/BigIntGlobalConfig.h
    include/yabil/bigint/Parallel.h
)

set(TESTS
    test/BigIntAddOperator_tests.cpp
    test/BigIntAndOperator_tests.cpp
    test/BigIntComparaison_tests.cpp
    test/BigIntConstructor_tests.cpp
    test/BigIntConversion_tests.cpp
    test/BigIntDivOperator_tests.cpp
    test/BigIntBitOperations_tests.cpp
    test/BigIntGreaterComparaison_tests.cpp
    test/BigIntIncrementDecrementOperator_tests.cpp
    test/BigIntLowerComparaison_tests.cpp
    test/BigIntMulOperator_tests.cpp
    test/BigIntNotOperator_tests.cpp
    test/BigIntOrOperator_tests.cpp
    test/ParallelOperations_tests.cpp
    test/BigIntShiftOperator_tests.cpp
    test/BigIntStreamOperator_tests.cpp
    test/BigIntSubOperator_tests.cpp
    test/BigIntXorOperator_tests.cpp
)

add_library(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE utils)

target_sources(${PROJECT_NAME}
    PRIVATE ${SOURCES}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES ${HEADERS}
)

set_common_properties(${PROJECT_NAME})
add_test_target(${PROJECT_NAME} ${TESTS})

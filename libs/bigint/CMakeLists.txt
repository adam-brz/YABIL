project(bigint C CXX)

set(SOURCES
    src/BigInt.cpp
    src/BigIntArithmeticOperators.cpp
    src/BigIntBinaryOperators.cpp
    src/BigIntRelationOperators.cpp
    src/io.cpp
    src/StringConversionUtils.cpp
    src/StringConversionUtils.h
    src/add_sub/AddSub.h
    src/add_sub/AddSub.cpp
    src/arithmetic/Mul.cpp
    src/arithmetic/Div.cpp
    src/arithmetic/Add.cpp
    src/arithmetic/Sub.cpp
    src/impl/StrConversion.h
    src/impl/StrConversion.cpp
    src/impl/Arithmetic.h
    src/impl/Arithmetic.cpp
)

set(HEADERS
    include/yabil/bigint/impl/BigIntImpl.h
    include/yabil/bigint/BigInt.h
    include/yabil/bigint/Sign.h
    include/yabil/bigint/BigIntBase.h
    include/yabil/bigint/BigIntGlobalConfig.h
    include/yabil/bigint/io.h
    include/yabil/bigint/arithmetic/Sub.h
    include/yabil/bigint/arithmetic/Add.h
    include/yabil/bigint/arithmetic/Mul.h
    include/yabil/bigint/arithmetic/Div.h
)

set(TESTS
    test/arithmetic/BigIntAddOperator_tests.cpp
    test/arithmetic/BigIntDivOperator_tests.cpp
    test/arithmetic/BigIntIncrementDecrementOperator_tests.cpp
    test/arithmetic/BigIntMulOperator_tests.cpp
    test/arithmetic/BigIntSubOperator_tests.cpp
    test/bit/BigIntAndOperator_tests.cpp
    test/bit/BigIntBitOperations_tests.cpp
    test/bit/BigIntNotOperator_tests.cpp
    test/bit/BigIntOrOperator_tests.cpp
    test/bit/BigIntShiftOperator_tests.cpp
    test/bit/BigIntXorOperator_tests.cpp
    test/relation/BigIntComparison_tests.cpp
    test/relation/BigIntGreaterComparison_tests.cpp
    test/relation/BigIntLowerComparison_tests.cpp
    test/BigIntConstructor_tests.cpp
    test/BigIntConversion_tests.cpp
    test/BigIntGlobalConfig_tests.cpp
    test/BigIntIsPowerOf2_tests.cpp
    test/BigIntStreamOperator_tests.cpp
)

set(BENCHMARKS
    benchmark/Addition.cpp
    benchmark/BenchmarkUtils.cpp
    benchmark/BenchmarkUtils.h
    benchmark/BitOperations.cpp
    benchmark/Division.cpp
    benchmark/Multiplication.cpp
    benchmark/StrConversion.cpp
    benchmark/Subtraction.cpp
)

if(YABIL_ENABLE_CUDA)
    list(APPEND SOURCES src/add_sub/AddSubCUDAImpl.cu)
elseif(YABIL_HAS_AVX512)
    list(APPEND SOURCES src/add_sub/AddSubAVX512DQImpl.cpp)
elseif(YABIL_HAS_AVX2)
    list(APPEND SOURCES src/add_sub/AddSubAVX2Impl.cpp)
else()
    list(APPEND SOURCES src/add_sub/AddSubCppImpl.cpp)
endif()

add_library(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PUBLIC utils)

if(YABIL_HAS_X64_INTRINSICS)
    target_compile_definitions(${PROJECT_NAME} PRIVATE YABIL_INTRINSICS_HEADER_FILE=<${YABIL_INTRINSICS_HEADER_FILE}>)
endif()

setup_algorithms_config_file()

target_sources(${PROJECT_NAME}
    PRIVATE ${SOURCES}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES ${HEADERS}
)

set_common_properties(${PROJECT_NAME})
add_test_target(${PROJECT_NAME} ${TESTS})
add_benchmark_target(${PROJECT_NAME} ${BENCHMARKS})

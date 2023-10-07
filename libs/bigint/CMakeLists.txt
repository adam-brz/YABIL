project(bigint C CXX)

set(SOURCES
    src/Arithmetic.cpp
    src/Arithmetic.h
    src/BigInt.cpp
    src/BigIntArithmeticOperators.cpp
    src/BigIntBinaryOperators.cpp
    src/BigIntGlobalConfig.cpp
    src/BigIntReleationOperators.cpp
    src/StringConversionUtils.cpp
    src/StringConversionUtils.h
    src/TypeUtils.h
    src/add_sub/AddSub.h
    src/add_sub/AddSub.cpp
    src/parallel/Parallel.cpp
    src/parallel/ParallelImpl.h
)

set(HEADERS
    include/yabil/bigint/BigInt.h
    include/yabil/bigint/BigIntBase.h
    include/yabil/bigint/BigIntGlobalConfig.h
    include/yabil/bigint/Parallel.h
    include/yabil/bigint/Thresholds.h
)

set(TESTS
    test/BigIntAddOperator_tests.cpp
    test/BigIntAndOperator_tests.cpp
    test/BigIntComparaison_tests.cpp
    test/BigIntConstructor_tests.cpp
    test/BigIntConversion_tests.cpp
    test/BigIntDivOperator_tests.cpp
    test/BigIntGlobalConfig_tests.cpp
    test/BigIntBitOperations_tests.cpp
    test/BigIntGreaterComparaison_tests.cpp
    test/BigIntIncrementDecrementOperator_tests.cpp
    test/BigIntIsPowerOf2_tests.cpp
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

if(YABIL_ENABLE_TBB)
    list(APPEND SOURCES src/parallel/ParallelTBBImpl.cpp)
else()
    list(APPEND SOURCES src/parallel/ParallelStdImpl.cpp)
endif()

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
target_link_libraries(${PROJECT_NAME} PUBLIC visibility PRIVATE utils)

if(YABIL_ENABLE_TBB)
    target_link_libraries(${PROJECT_NAME} PRIVATE TBB::tbb)
endif()

if(NOT YABIL_BIGINT_BASE_TYPE STREQUAL "auto")
    target_compile_definitions(${PROJECT_NAME} PUBLIC BIGINT_BASE_T=${YABIL_BIGINT_BASE_TYPE})
endif()

target_sources(${PROJECT_NAME}
    PRIVATE ${SOURCES}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES ${HEADERS}
)

set_common_properties(${PROJECT_NAME})
add_test_target(${PROJECT_NAME} ${TESTS})

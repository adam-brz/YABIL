project(compile_time C CXX)

set(HEADERS
    include/yabil/compile_time/ConstBigUInt.h
    include/yabil/compile_time/Concepts.h
)

set(TESTS
    test/ConstBigUInt_tests.cpp
)

add_library(${PROJECT_NAME} INTERFACE)

target_sources(${PROJECT_NAME}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES ${HEADERS}
)

set_common_properties(${PROJECT_NAME})
add_test_target(${PROJECT_NAME} ${TESTS})

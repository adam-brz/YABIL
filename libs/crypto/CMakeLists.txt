project(crypto C CXX)

set(SOURCES
    src/Utils.cpp
)

set(HEADERS
    include/yabil/crypto/Utils.h
)

set(TESTS
    test/UtilsFactorial_tests.cpp
)

add_library(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PUBLIC bigint)

target_sources(${PROJECT_NAME}
    PRIVATE ${SOURCES}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES ${HEADERS}
)

set_common_properties(${PROJECT_NAME})
add_test_target(${PROJECT_NAME} ${TESTS})

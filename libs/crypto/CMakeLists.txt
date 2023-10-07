project(crypto C CXX)

set(SOURCES
    src/Primes.cpp
    src/Primes.h
    src/Random.cpp
    src/RSA.cpp
)

set(HEADERS
    include/yabil/crypto/Random.h
    include/yabil/crypto/RSA.h
)

set(TESTS
    test/Random_tests.cpp
    test/RSA_tests.cpp
)

add_library(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PUBLIC bigint visibility PRIVATE math)

target_sources(${PROJECT_NAME}
    PRIVATE ${SOURCES}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES ${HEADERS}
)

set_common_properties(${PROJECT_NAME})
add_test_target(${PROJECT_NAME} ${TESTS})

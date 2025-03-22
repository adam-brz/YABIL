project(test_utils C CXX)

set(HEADERS
    include/yabil/test_utils/BenchmarkUtils.h
)

set(SOURCES
    src/BenchmarkUtils.cpp
)


add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE ${SOURCES}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES ${HEADERS}
)

set_target_properties(${PROJECT_NAME} PROPERTIES IS_TEST_TARGET TRUE)
set_common_properties(${PROJECT_NAME})


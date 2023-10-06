project(visibility C CXX)

add_library(${PROJECT_NAME} INTERFACE)

target_sources(${PROJECT_NAME}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES include/yabil/visibility/Visibility.h
)

set_common_properties(${PROJECT_NAME})

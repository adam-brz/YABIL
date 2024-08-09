cmake_minimum_required(VERSION 3.23)
project(yabil VERSION 0.1)

include(cmake/setup.cmake)
include(cmake/common.cmake)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
    message(STATUS "Setting CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
endif()

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD "20" CACHE STRING "Required c++ standard." FORCE)
    message(STATUS "Setting CMAKE_CXX_STANDARD = ${CMAKE_CXX_STANDARD}")
endif()

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(YABIL_ENABLE_SANITIZER "Enable address sanitizer" OFF)
option(YABIL_ENABLE_TESTS "Enable test targets" OFF)
option(YABIL_ENABLE_BENCHMARKS "Enable benchmark targets" OFF)
option(YABIL_ENABLE_TBB "Use TBB library" OFF)
option(YABIL_ENABLE_COVERAGE "Build with coverage info" OFF)
option(YABIL_ENABLE_CUDA "Enable CUDA" OFF)
option(YABIL_ENABLE_NATIVE_OPTIMIZATIONS "Enable optimizations using native instrucitons (AVX)" OFF)

set(YABIL_BIGINT_BASE_TYPE "auto" CACHE STRING "Numeric type to use as number base.")
set(CPACK_GENERATOR "ZIP" CACHE STRING "Generators to support. Semicolon delimited list.")

if(YABIL_ENABLE_CUDA AND NOT CMAKE_CUDA_STANDARD)
    set(CMAKE_CUDA_STANDARD "17" CACHE STRING "CUDA standard" FORCE)
    message(STATUS "Setting CMAKE_CUDA_STANDARD = ${CMAKE_CUDA_STANDARD}")
endif()

setup_deps()
setup_testing()
setup_compiler_options()

add_subdirectory(libs)

setup_install()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

message(STATUS "Additional configuration (cache): ")
message(STATUS "  YABIL_DEBUG_FLAGS:   ${YABIL_DEBUG_FLAGS}")
message(STATUS "  YABIL_RELEASE_FLAGS: ${YABIL_RELEASE_FLAGS}")
message(STATUS "  YABIL_LINK_FLAGS:    ${YABIL_LINK_FLAGS}")

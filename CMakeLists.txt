cmake_minimum_required(VERSION 3.23)
project(yabil VERSION 0.1)

include(cmake/common.cmake)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type." FORCE)
endif()

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(YABIL_ENABLE_SANITIZER "Enable address sanitizer" OFF)
option(YABIL_ENABLE_TESTS "Enable test targets" OFF)
option(YABIL_ENABLE_COVERAGE "Build with coverage info" OFF)

set(CPACK_GENERATOR "ZIP" CACHE STRING "Generators to support. Semicolon delimited list.")

setup_conan()
setup_testing()

add_subdirectory(libs)

setup_install()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

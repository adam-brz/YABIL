cmake_minimum_required(VERSION 3.20)
project(yabil VERSION 0.1)

include(cmake/common.cmake)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(YABIL_ENABLE_TESTS "Enable test targets" ON)
option(YABIL_USE_CONAN "Use Conan package manager for dependencies" ON)

set(CPACK_GENERATOR "ZIP" CACHE STRING "Generators to support. Semicolon delimited list.")

setup_conan()
setup_testing()

add_subdirectory(libs)

setup_install()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

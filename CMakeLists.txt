project(yabil)
cmake_minimum_required(VERSION 3.20)

include(cmake/common.cmake)

set(YABIL_ENABLE_TESTS OFF CACHE BOOL "Enable test targets.")
set(YABIL_USE_CONAN ON CACHE BOOL "Use Conan package manager for dependencies.")

setup_conan()
setup_testing()

add_subdirectory(libs)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

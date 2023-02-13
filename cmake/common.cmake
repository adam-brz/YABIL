macro(setup_testing)
    if(YABIL_ENABLE_TESTS)
        include(CTest)
        include(GoogleTest)
        enable_testing()
    endif()
endmacro()

macro(setup_conan)
    if(YABIL_USE_CONAN AND YABIL_ENABLE_TESTS)
        include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
    endif()
endmacro()

function(set_common_properties TARGET)
    set_target_properties(${TARGET} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED TRUE
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    if (MSVC)
        target_compile_options(${TARGET} PRIVATE /W4 $<IF:$<CONFIG:Debug>,/Zi,/O2>)
    else()
        target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Wpedantic $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0 -g>)
        target_link_options(${TARGET} PRIVATE $<$<CONFIG:RELEASE>:-s>)
    endif()

    if(BUILD_SHARED_LIBS AND MSVC)
        set_target_properties(${TARGET} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    endif()

    install(TARGETS ${TARGET}
        EXPORT "${CMAKE_PROJECT_NAME}Targets"
        FILE_SET HEADERS
    )
endfunction()

function(add_test_target TARGET)
    if(NOT YABIL_ENABLE_TESTS OR NOT ARGN)
        return()
    endif()

    set(TEST_TARGET ${TARGET}_tests)
    find_package(GTest REQUIRED CONFIG)

    add_executable(${TEST_TARGET} ${ARGN})
    target_link_libraries(${TEST_TARGET} PRIVATE ${TARGET} GTest::gtest GTest::gtest_main)

    if (MSVC)
        set_target_properties(${TEST_TARGET} PROPERTIES LINK_FLAGS "/ignore:4099")
    endif()

    set_common_properties(${TEST_TARGET})
    gtest_discover_tests(${TEST_TARGET})
endfunction()

function(setup_install)
    install(EXPORT "${CMAKE_PROJECT_NAME}Targets"
        FILE "${CMAKE_PROJECT_NAME}Targets.cmake"
        NAMESPACE ${CMAKE_PROJECT_NAME}::
        DESTINATION lib/cmake/${CMAKE_PROJECT_NAME}
    )

    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
        "${CMAKE_PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${CMAKE_PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
    )

    install(FILES "cmake/${CMAKE_PROJECT_NAME}Config.cmake" "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION lib/cmake/${CMAKE_PROJECT_NAME}
    )
endfunction()

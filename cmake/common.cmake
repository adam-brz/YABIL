# common.cmake

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(CheckCXXCompilerFlag)

macro(setup_testing)
    if(YABIL_ENABLE_TESTS)
        include(CTest)
        include(GoogleTest)
        enable_testing()
        find_package(GTest REQUIRED CONFIG)
    elseif(YABIL_ENABLE_COVERAGE)
        message(WARNING "Option YABIL_ENABLE_COVERAGE=TRUE will be ignored since YABIL_ENABLE_TESTS=FALSE.")
    endif()
endmacro()

macro(setup_conan)
    if(YABIL_ENABLE_TESTS)
        include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
    endif()
endmacro()

function(set_common_properties TARGET)
    set_target_properties(${TARGET} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED TRUE
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    find_package(OpenMP)
    if(OpenMP_CXX_FOUND)
        target_link_libraries(${TARGET} PRIVATE OpenMP::OpenMP_CXX)
    endif()

    add_coverage(${TARGET})
    target_include_directories(${TARGET} PRIVATE src)
    set_common_target_options(${TARGET})

    get_target_property(IS_TEST_TARGET ${TARGET} IS_TEST_TARGET)
    if(NOT IS_TEST_TARGET)
        install(TARGETS ${TARGET}
            EXPORT "${CMAKE_PROJECT_NAME}Targets"
            FILE_SET HEADERS
        )
    endif()
endfunction()

function(set_common_target_options TARGET)
    set(MSVC_DEBUG_FLAGS /Zi /Od)
    set(MSVC_RELEASE_FLAGS /O2)
    set(OTHER_DEBUG_FLAGS -O0 -g -Wno-pragmas)
    set(OTHER_RELEASE_FLAGS -O3 -Wno-pragmas)
    set(OTHER_DEBUG_LINK_FLAGS "")

    if(YABIL_ENABLE_SANITIZER)
        set(OTHER_DEBUG_FLAGS ${OTHER_DEBUG_FLAGS} -fsanitize=address -fno-omit-frame-pointer)
        set(OTHER_DEBUG_LINK_FLAGS ${OTHER_DEBUG_LINK_FLAGS} -fsanitize=address)
    endif()

    if(YABIL_ENABLE_OPTIMIZATIONS)
        check_cxx_compiler_flag("-Ofast" OFAST_SUPPORTED)
        check_cxx_compiler_flag("-march=native" MNATIVE_SUPPORTED)

        if(OFAST_SUPPORTED)
            set(OTHER_RELEASE_FLAGS ${OTHER_RELEASE_FLAGS} -Ofast)
        endif()

        if(MNATIVE_SUPPORTED)
            set(OTHER_RELEASE_FLAGS ${OTHER_RELEASE_FLAGS} -march=native)
        endif()
    endif()

    if (MSVC)
        target_compile_options(${TARGET} PRIVATE /W4 /wd4068)
    else()
        target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    if(CMAKE_CONFIGURATION_TYPES)
        if (MSVC)
            target_compile_options(${TARGET} PRIVATE $<$<CONFIG:Debug>:${MSVC_DEBUG_FLAGS}>)
        else()
            target_compile_options(${TARGET} PRIVATE
                $<$<CONFIG:Release>:${OTHER_RELEASE_FLAGS}>
                $<$<CONFIG:Debug>:${OTHER_DEBUG_FLAGS}>
            )
        endif()
    else()
        if(CMAKE_BUILD_TYPE STREQUAL "Release")
            target_compile_options(${TARGET} PRIVATE ${OTHER_RELEASE_FLAGS})
        else()
            target_compile_options(${TARGET} PRIVATE ${OTHER_DEBUG_FLAGS})
            target_link_options(${TARGET} PRIVATE ${OTHER_DEBUG_LINK_FLAGS})
        endif()
    endif()

    if(BUILD_SHARED_LIBS AND MSVC)
        set_target_properties(${TARGET} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    endif()
endfunction()


function(add_coverage TARGET)
    if(YABIL_ENABLE_COVERAGE)
        if(NOT (CMAKE_CXX_COMPILER MATCHES "clang"))
            message(ERROR "Coverage is only supported for clang compiler")
        endif()
        target_compile_options(${TARGET} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
        target_link_options(${TARGET} PRIVATE -fprofile-instr-generate)
    endif()
endfunction()

function(add_test_target TARGET)
    if(NOT YABIL_ENABLE_TESTS OR NOT ARGN)
        return()
    endif()

    set(TEST_TARGET ${TARGET}_tests)
    add_executable(${TEST_TARGET} ${ARGN})
    target_link_libraries(${TEST_TARGET} PRIVATE ${TARGET} GTest::gtest GTest::gtest_main)

    set_target_properties(${TEST_TARGET} PROPERTIES IS_TEST_TARGET TRUE)
    setup_test_target(${TEST_TARGET})
endfunction()

function(setup_test_target TEST_TARGET)
    if (MSVC)
        set_target_properties(${TEST_TARGET} PROPERTIES LINK_FLAGS "/ignore:4099")
    endif()

    set_common_properties(${TEST_TARGET})
    if(YABIL_ENABLE_COVERAGE)
        get_target_property(EXE_OUTPUT_DIR ${TEST_TARGET} RUNTIME_OUTPUT_DIRECTORY)
        add_test(
            NAME ${TEST_TARGET}
            COMMAND ${CMAKE_COMMAND} -E env LLVM_PROFILE_FILE=${TEST_TARGET}_%m.profraw -- ${EXE_OUTPUT_DIR}/${TEST_TARGET}${CMAKE_EXECUTABLE_SUFFIX}
            WORKING_DIRECTORY ${EXE_OUTPUT_DIR}
        )
    else()
        gtest_discover_tests(${TEST_TARGET} DISCOVERY_TIMEOUT 30)
    endif()
endfunction()

function(setup_install)
    install(EXPORT "${CMAKE_PROJECT_NAME}Targets"
        FILE "${CMAKE_PROJECT_NAME}Targets.cmake"
        NAMESPACE ${CMAKE_PROJECT_NAME}::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}"
    )

    configure_package_config_file(
        "cmake/config.cmake.in"
        "${CMAKE_PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}"
        PATH_VARS CMAKE_INSTALL_LIBDIR
    )

    write_basic_package_version_file(
        "${CMAKE_PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${CMAKE_PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    install(
        FILES
            "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION
            "${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}"
    )
endfunction()

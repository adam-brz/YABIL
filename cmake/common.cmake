# common.cmake

function(set_common_properties TARGET)
    set_target_properties(${TARGET} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED TRUE
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib$<0:>"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib$<0:>"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin$<0:>"
    )

    setup_install_rule(${TARGET})

    get_target_property(TARGET_TYPE ${TARGET} TYPE)
    if (${TARGET_TYPE} STREQUAL "INTERFACE_LIBRARY")
        return()
    endif()

    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src)
        target_include_directories(${TARGET} PRIVATE src)
    endif()

    if(YABIL_ENABLE_CUDA)
        enable_language(CUDA)
        set_target_properties(${TARGET} PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
        if(APPLE)
            set_target_properties(${TARGET} PROPERTIES BUILD_RPATH ${CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES})
        endif()
    endif()

    add_coverage(${TARGET})
    set_common_target_options(${TARGET})
endfunction()

function(setup_install_rule TARGET)
    get_target_property(IS_TEST_TARGET ${TARGET} IS_TEST_TARGET)
    if(NOT IS_TEST_TARGET)
        install(TARGETS ${TARGET}
            EXPORT "${CMAKE_PROJECT_NAME}Targets"
            FILE_SET HEADERS
        )
    endif()
endfunction()

function(set_common_target_options TARGET)
    set(MSVC_FLAGS /W4 /wd4068)
    set(OTHER_FLAGS -Wall -Wextra -Wpedantic -Wno-pragmas)
    set(OTHER_DEBUG_LINK_FLAGS "")

    if(YABIL_ENABLE_SANITIZER)
        set(YABIL_SANITIZER_TYPE "address" CACHE STRING "Sanitizer to use (eg. address, thread, memory)")
        set(SANITIZER_COMPILE_FLAGS -fsanitize=${YABIL_SANITIZER_TYPE} -fno-omit-frame-pointer)
        set(SANITIZER_LINK_FLAGS -fsanitize=${YABIL_SANITIZER_TYPE})
    endif()

    if(YABIL_MNATIVE_SUPPORTED)
        set(MNATIVE_FLAG -march=native)
    endif()

    set(YABIL_MSVC_DEBUG_FLAGS /Zi /Od ${MSVC_FLAGS} CACHE STRING "Additional MSVC compile flags for Debug configuration.")
    set(YABIL_MSVC_RELEASE_FLAGS ${MSVC_FLAGS} CACHE STRING "Additional MSVC compile flags for Release configuration.")
    set(YABIL_DEBUG_FLAGS -O0 -g ${OTHER_FLAGS} ${SANITIZER_COMPILE_FLAGS} ${MNATIVE_FLAG} CACHE STRING "Additional compile flags for Debug configuration.")
    set(YABIL_RELEASE_FLAGS -O3 ${OTHER_FLAGS} ${SANITIZER_COMPILE_FLAGS} ${MNATIVE_FLAG} CACHE STRING "Additional compile flags for Release configuration.")
    set(YABIL_LINK_FLAGS ${SANITIZER_LINK_FLAGS} CACHE STRING "Additional compile flags for Release configuration.")

    if(CMAKE_CONFIGURATION_TYPES)
        if (MSVC)
            target_compile_options(${TARGET} PRIVATE
                $<$<CONFIG:Release>:${YABIL_MSVC_RELEASE_FLAGS}>
                $<$<CONFIG:Debug>:${YABIL_MSVC_DEBUG_FLAGS}>
            )
        else()
            target_compile_options(${TARGET} PRIVATE
                $<$<CONFIG:Release>:${YABIL_RELEASE_FLAGS}>
                $<$<CONFIG:Debug>:${YABIL_DEBUG_FLAGS}>
            )
        endif()
    else()
        if(CMAKE_BUILD_TYPE STREQUAL "Release")
            target_compile_options(${TARGET} PRIVATE ${YABIL_RELEASE_FLAGS})
        else()
            target_compile_options(${TARGET} PRIVATE ${YABIL_DEBUG_FLAGS})
        endif()
        target_link_options(${TARGET} PRIVATE ${YABIL_LINK_FLAGS})
    endif()

    if(BUILD_SHARED_LIBS)
        target_compile_definitions(${TARGET} PUBLIC YABIL_DLL)
        target_compile_definitions(${TARGET} PRIVATE YABIL_DLL_EXPORTS)
    endif()
endfunction()

function(add_coverage TARGET)
    if(YABIL_ENABLE_COVERAGE)
        if(NOT (CMAKE_CXX_COMPILER MATCHES "clang"))
            message(ERROR "Currently coverage is only supported for clang compiler")
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
    else()
        if(CMAKE_BUILD_TYPE STREQUAL "Release" AND NOT YABIL_ENABLE_GPERFTOOLS)
            add_custom_command(
                TARGET ${TEST_TARGET} POST_BUILD
                COMMAND ${CMAKE_STRIP}
                ARGS --strip-all $<TARGET_FILE:${TEST_TARGET}>
            )
        endif()
    endif()

    set_common_properties(${TEST_TARGET})

    if(CMAKE_CROSSCOMPILING)
        return()
    endif()

    if(YABIL_ENABLE_COVERAGE)
        get_target_property(EXE_OUTPUT_DIR ${TEST_TARGET} RUNTIME_OUTPUT_DIRECTORY)
        add_test(
            NAME ${TEST_TARGET}
            COMMAND ${CMAKE_COMMAND} -E env LLVM_PROFILE_FILE=${TEST_TARGET}_%m.profraw -- $<TARGET_FILE:${TEST_TARGET}>
            WORKING_DIRECTORY ${EXE_OUTPUT_DIR}
        )
    else()
        gtest_discover_tests(${TEST_TARGET} DISCOVERY_TIMEOUT 30)
    endif()
endfunction()

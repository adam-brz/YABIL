# common.cmake

include(GenerateExportHeader)

function(set_common_properties TARGET)
    set_target_properties(${TARGET} PROPERTIES
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
        set_target_properties(${TARGET} PROPERTIES
            # CUDA_SEPARABLE_COMPILATION ON
            CUDA_RESOLVE_DEVICE_SYMBOLS ON
        )
    endif()

    setup_export_header(${TARGET})
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

function(setup_export_header TARGET)
    get_target_property(IS_TEST_TARGET ${TARGET} IS_TEST_TARGET)
    if(NOT IS_TEST_TARGET)
        set(EXPORT_FILE_DIR "${CMAKE_PROJECT_NAME}/${TARGET}")
        set(EXPORT_FILE_NAME "${CMAKE_PROJECT_NAME}/${TARGET}/${TARGET}_export.h")
        string(TOUPPER "${CMAKE_PROJECT_NAME}_" EXPORT_MACRO_PREFIX)
        generate_export_header(${TARGET} PREFIX_NAME ${EXPORT_MACRO_PREFIX} EXPORT_FILE_NAME ${EXPORT_FILE_NAME})
        target_include_directories(${TARGET} PUBLIC
            $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
            $<INSTALL_INTERFACE:include>
        )
        install(FILES ${PROJECT_BINARY_DIR}/${EXPORT_FILE_NAME} DESTINATION include/${EXPORT_FILE_DIR})
    endif()
endfunction()

function(set_common_target_options TARGET)
    message(STATUS "Setting up target: ${TARGET}")

    set(MSVC_FLAGS /W4 /wd4068)
    set(OTHER_FLAGS -Wall -Wextra -Wpedantic -Wno-pragmas)

    if(YABIL_ENABLE_SANITIZER)
        set(YABIL_SANITIZER_TYPE "address" CACHE STRING "Sanitizer to use (eg. address, thread, memory)")
        set(SANITIZER_COMPILE_FLAGS -fsanitize=${YABIL_SANITIZER_TYPE} -fno-omit-frame-pointer)
        set(SANITIZER_LINK_FLAGS -fsanitize=${YABIL_SANITIZER_TYPE})
    endif()

    if(YABIL_MNATIVE_SUPPORTED)
        set(MNATIVE_FLAG -march=native)
    endif()

    setup_coverage_variables()

    if (MSVC)
        set(YABIL_DEBUG_FLAGS /Zi /Od ${MSVC_FLAGS} ${SANITIZER_COMPILE_FLAGS} ${MNATIVE_FLAG} CACHE STRING "Additional compile flags for Debug configuration.")
        set(YABIL_RELEASE_FLAGS ${MSVC_FLAGS} ${SANITIZER_COMPILE_FLAGS} ${MNATIVE_FLAG} CACHE STRING "Additional compile flags for Release configuration.")
        set(YABIL_LINK_FLAGS "" CACHE STRING "Additional compile flags for Release configuration.")
    else()
        set(YABIL_DEBUG_FLAGS -O0 -g ${OTHER_FLAGS} ${SANITIZER_COMPILE_FLAGS} ${MNATIVE_FLAG} ${COVERAGE_COMPILE_FLAGS} CACHE STRING "Additional compile flags for Debug configuration.")
        set(YABIL_RELEASE_FLAGS -O3 ${OTHER_FLAGS} ${SANITIZER_COMPILE_FLAGS} ${MNATIVE_FLAG} ${COVERAGE_COMPILE_FLAGS} CACHE STRING "Additional compile flags for Release configuration.")
        set(YABIL_LINK_FLAGS ${SANITIZER_LINK_FLAGS} ${COVERAGE_LINK_FLAGS} CACHE STRING "Additional compile flags for Release configuration.")
    endif()

    if(CMAKE_CONFIGURATION_TYPES)
        target_compile_options(${TARGET} PRIVATE
            $<$<COMPILE_LANGUAGE:CXX>:
                $<$<CONFIG:Release>:${YABIL_RELEASE_FLAGS}>
                $<$<CONFIG:Debug>:${YABIL_DEBUG_FLAGS}>
            >
        )
        target_link_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${YABIL_LINK_FLAGS}>)
    else()
        if(CMAKE_BUILD_TYPE STREQUAL "Release")
            target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${YABIL_RELEASE_FLAGS}>)
        else()
            target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${YABIL_DEBUG_FLAGS}>)
        endif()
        target_link_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${YABIL_LINK_FLAGS}>)
    endif()

    if(NOT BUILD_SHARED_LIBS)
        string(TOUPPER "${TARGET}" UPPER_TARGET_NAME)
        target_compile_definitions(${TARGET} PUBLIC YABIL_${UPPER_TARGET_NAME}_STATIC_DEFINE)
    endif()
endfunction()

macro(setup_coverage_variables)
    if(YABIL_ENABLE_COVERAGE)
        if(NOT (CMAKE_CXX_COMPILER MATCHES "clang"))
            message(ERROR "Currently coverage is only supported for clang compiler")
        endif()
        set(COVERAGE_COMPILE_FLAGS -fprofile-instr-generate -fcoverage-mapping)
        set(COVERAGE_LINK_FLAGS -fprofile-instr-generate)
    endif()
endmacro()

function(add_test_target TARGET)
    if(NOT YABIL_ENABLE_TESTS OR NOT ARGN)
        return()
    endif()

    set(TEST_TARGET ${TARGET}_tests)
    add_executable(${TEST_TARGET} ${ARGN})
    target_link_libraries(${TEST_TARGET} PRIVATE ${TARGET} GTest::gtest GTest::gtest_main)

    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(${TEST_TARGET} PRIVATE -fconstexpr-backtrace-limit=0 -fconstexpr-steps=4194304)
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        target_compile_options(${TEST_TARGET} PRIVATE -fconstexpr-steps=4194304)
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        target_compile_options(${TEST_TARGET} PRIVATE /constexpr:steps4194304)
    endif()

    if(YABIL_ENABLE_CUDA)
        target_link_libraries(${TEST_TARGET} PRIVATE CUDA::cudart)
    endif()

    set_target_properties(${TEST_TARGET} PROPERTIES IS_TEST_TARGET TRUE)
    setup_test_target(${TEST_TARGET})
endfunction()

function(setup_test_target TEST_TARGET)
    if (MSVC)
        set_target_properties(${TEST_TARGET} PROPERTIES LINK_FLAGS "/ignore:4099")
    elseif(APPLE)
        if(CMAKE_BUILD_TYPE STREQUAL "Release")
            add_custom_command(
                TARGET ${TEST_TARGET} POST_BUILD
                COMMAND ${CMAKE_STRIP}
                ARGS -nu $<TARGET_FILE:${TEST_TARGET}>
            )
        endif()
    else()
        if(CMAKE_BUILD_TYPE STREQUAL "Release")
            add_custom_command(
                TARGET ${TEST_TARGET} POST_BUILD
                COMMAND ${CMAKE_STRIP}
                ARGS --strip-all $<TARGET_FILE:${TEST_TARGET}>
            )
        endif()
    endif()

    set_common_properties(${TEST_TARGET})

    if(YABIL_ENABLE_CUDA)
        if(APPLE)
            set_target_properties(${TEST_TARGET} PROPERTIES BUILD_RPATH ${CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES})
        endif()
    endif()

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

function(setup_algorithms_config_file)
    set(YABIL_CONFIG_KARATSUBA_THRESHOLD "64" CACHE STRING "")
    set(YABIL_CONFIG_RECURSIVE_DIV_THRESHOLD "1200" CACHE STRING "")
    set(YABIL_CONFIG_PARALLEL_ADD_THRESHOLD "2000" CACHE STRING "")
    set(YABIL_CONFIG_PARALLEL_MUL_THRESHOLD "256" CACHE STRING "")
    set(YABIL_CONFIG_PARALLEL_DIV_THRESHOLD "1800" CACHE STRING "")

    set(YABIL_CONFIG_CONSTEVAL_THRESHOLDS "1" CACHE STRING "Boolean value indicating if algorithm configuration should only be known in compile time.")
    set(YABIL_CONFIG_USE_CONSTEVAL_AUTO_PARALLEL "0" CACHE STRING "Boolean value indicating if parallel configuration should only be known in compile time.")
    set(YABIL_CONFIG_AUTO_PARALLEL_ENABLED "1" CACHE STRING "Boolean value indicating if parallel algorithms should be enabled when parallel is fixed in compile time.")

    set(CONFIG_FILE_DIR ${CMAKE_PROJECT_NAME}/${PROJECT_NAME})
    set(FULL_CONFIG_PATH ${PROJECT_BINARY_DIR}/${CONFIG_FILE_DIR}/algorithms_config.h)

    configure_file(${CMAKE_SOURCE_DIR}/cmake/algorithms_config.h.in ${FULL_CONFIG_PATH})

    target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>
    )
    install(FILES ${FULL_CONFIG_PATH} DESTINATION include/${CONFIG_FILE_DIR})

endfunction()

# setup.cmake

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(CheckCXXCompilerFlag)
include(CheckCXXSymbolExists)

macro(setup_deps)
    set(REQUIRED_FIND_PACKAGES_LIST "")
    if(YABIL_ENABLE_TESTS)
        find_package(GTest REQUIRED)
    endif()
    if(YABIL_ENABLE_TBB)
        find_package(TBB REQUIRED)
        list(APPEND REQUIRED_FIND_PACKAGES_LIST "find_package(TBB REQUIRED)\n")
    endif()
    if(YABIL_ENABLE_CUDA)
        find_package(CUDAToolkit REQUIRED)
    endif()
endmacro()

macro(setup_testing)
    if(YABIL_ENABLE_TESTS)
        include(CTest)
        include(GoogleTest)
        enable_testing()
    elseif(YABIL_ENABLE_COVERAGE)
        message(WARNING "Option YABIL_ENABLE_COVERAGE=TRUE will be ignored since YABIL_ENABLE_TESTS=FALSE.")
    endif()
endmacro()

macro(setup_compiler_options)
    if(YABIL_ENABLE_NATIVE_OPTIMIZATIONS)
        check_cxx_compiler_flag("-march=native" YABIL_MNATIVE_SUPPORTED)
        if(YABIL_MNATIVE_SUPPORTED)
            set(CMAKE_REQUIRED_FLAGS "-march=native")
            check_cxx_symbol_exists(__AVX2__ "" YABIL_HAS_AVX2)
            check_cxx_symbol_exists(__AVX512DQ__ "" YABIL_HAS_AVX512)
            set(YABIL_HAS_AVX2 "${YABIL_HAS_AVX2}" CACHE BOOL "Enable avx2 support")
            set(YABIL_HAS_AVX512 "${YABIL_HAS_AVX512}" CACHE BOOL "Enable avx512 support")
        endif()
    endif()
endmacro()

function(setup_install)
    string(REPLACE ";" " " REQUIRED_FIND_PACKAGES "${REQUIRED_FIND_PACKAGES_LIST}")
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

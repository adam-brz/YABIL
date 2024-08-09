from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.files import copy
from conan.tools.build import cppstd_flag, supported_cppstd, check_min_cppstd

import shutil
from pathlib import Path


class YabilConan(ConanFile):
    name = "yabil"
    license = "MIT"
    url = "https://github.com/Andrew2a1/YABIL"
    description = "YABIL - Yet Another Big Integer Library"
    topics = ("bigint", "integer", "cpp20")
    settings = "os", "compiler", "build_type", "arch"
    package_type = "library"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "native_optimizations": [True, False],
        "digit_type": ["uint16_t", "uint32_t", "uint64_t", "auto"],
        "with_tbb": [True, False],
        "with_tests": [True, False],
        "with_benchmarks": [True, False],
        "with_cuda": [True, False],  # experimental
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "native_optimizations": True,
        "digit_type": "auto",
        "with_tbb": True,
        "with_tests": False,
        "with_benchmarks": False,
        "with_cuda": False,
    }

    def export_sources(self):
        sources = Path(self.recipe_folder)
        export_dir = Path(self.export_sources_folder)
        copy(self, "CMakeLists.txt", sources, export_dir)
        copy(self, "LICENSE.txt", sources, export_dir)
        copy(self, "*", sources / "libs", export_dir / "libs")
        copy(self, "*", sources / "cmake", export_dir / "cmake")

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def validate(self):
        check_min_cppstd(self, "20")

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        if self.options.with_tbb:
            self.requires("onetbb/[>=2021.9.0]")
        if self.options.with_tests:
            self.test_requires("gtest/[>=1.13.0]")
        if self.options.with_benchmarks:
            self.test_requires("benchmark/[>=1.8.4]")

    def generate(self):
        self._import_dependencies()
        tc = CMakeToolchain(self)
        tc.variables["YABIL_ENABLE_TESTS"] = self.options.with_tests
        tc.variables["YABIL_ENABLE_BENCHMARKS"] = self.options.with_benchmarks
        tc.variables["YABIL_ENABLE_TBB"] = self.options.with_tbb
        tc.variables["YABIL_ENABLE_NATIVE_OPTIMIZATIONS"] = (
            self.options.native_optimizations
        )
        tc.variables["YABIL_ENABLE_CUDA"] = self.options.with_cuda
        tc.variables["YABIL_BIGINT_BASE_TYPE"] = self.options.digit_type
        tc.generate()
        CMakeDeps(self).generate()

    def _import_dependencies(self):
        bin_dir = Path(self.build_folder) / "bin"
        for dep in self.dependencies.values():
            copy(self, "*.dylib", dep.cpp_info.bindirs[0], bin_dir)
            copy(self, "*.dll", dep.cpp_info.bindirs[0], bin_dir)
            copy(self, "*.so*", dep.cpp_info.bindirs[0], bin_dir)

    def build(self):
        cmake = CMake(self)
        cmake.configure(variables={"CMAKE_VERBOSE_MAKEFILE": True})
        cmake.build()

        if self.options.with_tests:
            if ctest := shutil.which("ctest"):
                self.run(f"{ctest} -C {self.settings.build_type} --output-on-failure")

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "yabil")
        lib_components = ["bigint", "math", "crypto", "utils"]

        for conan_component in lib_components:
            self.cpp_info.components[conan_component].set_property(
                "cmake_target_name", f"yabil::{conan_component}"
            )
            self.cpp_info.components[conan_component].set_property(
                "cmake_file_name", conan_component
            )

            self.cpp_info.components[conan_component].libs = [conan_component]

            if not self.options.shared:
                self.cpp_info.components[conan_component].defines.append(
                    f"YABIL_{conan_component.upper()}_STATIC_DEFINE"
                )

        header_only_components = ["compile_time"]
        for conan_component in header_only_components:
            self.cpp_info.components[conan_component].set_property(
                "cmake_target_name", f"yabil::{conan_component}"
            )
            self.cpp_info.components[conan_component].set_property(
                "cmake_file_name", conan_component
            )

        if self.settings.os in ["Linux", "FreeBSD"]:
            self.cpp_info.components["utils"].system_libs = ["pthread"]

        self.cpp_info.components["bigint"].requires = ["utils"]
        self.cpp_info.components["math"].requires = ["bigint"]
        self.cpp_info.components["crypto"].requires = ["bigint", "math"]

        if self.options.digit_type != "auto":
            self.cpp_info.components["bigint"].defines.append(
                f"YABIL_BIGINT_BASE_T={self.options.digit_type}"
            )

        if self.options.with_tbb:
            self.cpp_info.components["bigint"].requires.append("onetbb::libtbb")

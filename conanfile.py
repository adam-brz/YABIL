from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.files import copy

from pathlib import Path

required_conan_version = ">=1.54.0"


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
        "with_cuda": [True, False],  # experimental
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "native_optimizations": True,
        "digit_type": "auto",
        "with_tbb": True,
        "with_tests": False,
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

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        if self.options.with_tbb:
            self.requires("onetbb/[>=2021.9.0]")
        if self.options.with_tests:
            self.test_requires("gtest/[>=1.13.0]")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["YABIL_ENABLE_TESTS"] = self.options.with_tests
        tc.variables["YABIL_ENABLE_TBB"] = self.options.with_tbb
        tc.variables[
            "YABIL_ENABLE_NATIVE_OPTIMIZATIONS"
        ] = self.options.native_optimizations
        tc.variables["YABIL_ENABLE_CUDA"] = self.options.with_cuda
        tc.variables["YABIL_BIGINT_BASE_TYPE"] = self.options.digit_type
        tc.generate()
        CMakeDeps(self).generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        if self.options.with_tests:
            self.run(f"ctest -C {str(self.settings.build_type).capitalize()} --verbose")

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "yabil")
        all_components = ["bigint", "math", "crypto", "utils"]

        for conan_component in all_components:
            self.cpp_info.components[conan_component].set_property(
                "cmake_target_name", f"yabil::{conan_component}"
            )
            self.cpp_info.components[conan_component].set_property(
                "cmake_file_name", conan_component
            )

            self.cpp_info.components[conan_component].libs = [conan_component]

        if self.settings.os in ["Linux", "FreeBSD"]:
            self.cpp_info.components["utils"].system_libs = ["pthread"]
            self.cpp_info.components["bigint"].requires = ["omp"]

        self.cpp_info.components["bigint"].requires = ["utils"]
        self.cpp_info.components["math"].requires = ["bigint"]
        self.cpp_info.components["crypto"].requires = ["bigint", "math"]

        if self.options.digit_type != "auto":
            self.cpp_info.components["bigint"].defines.append(
                f"BIGINT_BASE_T={self.options.digit_type}"
            )

        if self.options.shared:
            for conan_component in all_components:
                self.cpp_info.components[conan_component].defines.append("YABIL_DLL")

        if self.options.with_tbb:
            self.cpp_info.components["bigint"].requires.append("onetbb::libtbb")

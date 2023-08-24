from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.files import copy

import os
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
        "optimizations": [True, False],
        "digit_type": ["uint16_t", "uint32_t", "uint64_t", "auto"],
        "tbb": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "optimizations": False,
        "digit_type": "auto",
        "tbb": False,
    }

    def export_sources(self):
        sources = Path(self.recipe_folder).parent.parent
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
        if self.options.tbb:
            self.requires("onetbb/[>=2021.9.0]")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["YABIL_ENABLE_TESTS"] = False
        tc.variables["YABIL_ENABLE_TBB"] = self.options.tbb
        tc.variables["YABIL_ENABLE_OPTIMIZATIONS"] = self.options.optimizations
        tc.variables["YABIL_BIGINT_BASE_TYPE"] = self.options.digit_type
        tc.generate()
        CMakeDeps(self).generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "yabil")

        for conan_component in ["bigint", "math", "crypto", "utils"]:
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

        if self.options.tbb:
            self.cpp_info.components["bigint"].requires.append("onetbb::libtbb")

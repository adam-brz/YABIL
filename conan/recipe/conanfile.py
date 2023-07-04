from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.files import copy

import os

required_conan_version = ">=1.54.0"


class YabilConan(ConanFile):
    name = "yabil"
    version = "0.1"
    license = "MIT"
    url = "https://github.com/Andrew2a1/YABIL"
    description = "YABIL - Yet Another Big Integer Library"
    topics = ("bigint", "integer", "cpp20")
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "optimizations": [True, False],
        "tbb": [True, False]
    }
    default_options = {"shared": False, "fPIC": True, "optimizations": False, "tbb": False}
    exports_sources = "../../*"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

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
        tc.generate()

        CMakeDeps(self).generate()

        bindir = os.path.join(self.build_folder, "bin")
        for dep in self.dependencies.values():
            copy(self, "*.dll", dep.cpp_info.bindirs[0], bindir)
            copy(self, "*.dylib", dep.cpp_info.libdirs[0], bindir)

    # def source(self):
    #     self.run("git clone --depth 1 https://github.com/Andrew2a1/YABIL.git .")

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

        if self.options.tbb:
            self.cpp_info.components["bigint"].requires.append("TBB::tbb")

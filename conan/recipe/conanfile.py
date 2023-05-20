from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout


required_conan_version = ">=1.54.0"


class YabilConan(ConanFile):
    name = "yabil"
    version = "0.1"
    license = "MIT"
    url = "https://github.com/Andrew2a1/YABIL"
    description = "YABIL - Yet Another Big Integer Library"
    topics = ("bigint", "integer", "cpp17")
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "optimizations": [True, False],
    }
    default_options = {"shared": False, "fPIC": True, "optimizations": False}
    exports_sources = "../../*"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["YABIL_ENABLE_TESTS"] = False
        tc.variables["YABIL_ENABLE_OPTIMIZATIONS"] = self.options.optimizations
        tc.generate()

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

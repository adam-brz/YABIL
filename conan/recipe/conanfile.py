from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout


required_conan_version = ">=1.54.0"


class YabilConan(ConanFile):
    name = "yabil"
    version = "0.1"
    license = "MIT"
    url = "https://github.com/Andrew2a1/YABIL"
    description = "YABIL - Yet Another Big Integer Library"
    topics = ("bigint", "integer", "cpp17")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    generators = "CMakeToolchain", "CMakeDeps"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def layout(self):
        cmake_layout(self, src_folder="yabil")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["YABIL_ENABLE_TESTS"] = "OFF"
        tc.generate()
        CMakeDeps(self).generate()

    def source(self):
        self.run("git clone --depth 1 https://github.com/Andrew2a1/YABIL.git .")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "yabil")

        for conan_component in ["bigint", "crypto"]:
            self.cpp_info.components[conan_component].set_property(
                "cmake_target_name", f"yabil::{conan_component}"
            )
            self.cpp_info.components[conan_component].set_property(
                "cmake_file_name", conan_component
            )
            self.cpp_info.components[conan_component].names[
                "cmake_find_package"
            ] = conan_component
            self.cpp_info.components[conan_component].names[
                "cmake_find_package_multi"
            ] = conan_component
            self.cpp_info.components[conan_component].libs = ["bigint"]

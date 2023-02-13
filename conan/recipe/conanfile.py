from conans import ConanFile
from conan.tools.cmake import CMake, cmake_layout


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
    generators = "CMakeToolchain"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def layout(self):
        cmake_layout(self, src_folder="yabil")

    def source(self):
        self.run("git clone --depth 1 https://github.com/Andrew2a1/YABIL.git .")

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.configure(variables={"YABIL_ENABLE_TESTS": "OFF"})
        return cmake

    def package_info(self):
        self.cpp_info.libs = ["bigint", "crypto"]

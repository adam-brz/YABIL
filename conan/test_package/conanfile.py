import os, platform
from conans import ConanFile, CMake, tools


class YabilTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy("*.so*", dst="bin", src="lib")

    def test(self):
        if not tools.cross_building(self):
            os.chdir(f"bin/{self.settings.build_type}")
            self.run(
                f".{os.sep}PackageTest{'.exe' if platform.system() == 'Windows' else ''}"
            )

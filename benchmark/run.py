#!/usr/bin/python

import subprocess

subprocess.check_call("conan export ../conan/recipe", shell=True)
subprocess.check_call("conan install . -of=build -pr ../conan/profiles/clang -s build_type=Release -o shared=False --build=missing", shell=True)
subprocess.check_call("cmake --preset conan-release -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_EXPORT_COMPILE_COMMANDS=ON", shell=True)
subprocess.check_call("cmake --build ./build -j 12", shell=True)
subprocess.check_call("./build/yabil_benchmarks", shell=True)

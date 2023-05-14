#!/usr/bin/python

import os, sys
import platform
import subprocess
import shutil

results_output = "./build/results.json"

os.makedirs("build", exist_ok=True)

subprocess.check_call("conan export ../conan/recipe", shell=True)

if platform.system() == "Linux":
    profile="clang"
    additional_flags="-c tools.system.package_manager:mode=install"
    build_flags="-DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang"
elif platform.system() == "Darwin":
    profile="macos"
    additional_flags=""
    build_flags=""

subprocess.check_call(
    f"conan install .. -of=. -pr ../../conan/profiles/{profile} -s build_type=Release --build=missing {additional_flags}",
    shell=True,
    cwd="build",
)

shutil.copyfile("build/CMakePresets.json", "CMakePresets.json")

subprocess.check_call(
    f"cmake --preset release {build_flags} -DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
    shell=True,
)

subprocess.check_call("cmake --build ./build -j 12", shell=True)

if len(sys.argv) > 1:
    filter_flag = f"--benchmark_filter={sys.argv[1]}"
else:
    filter_flag = ""

subprocess.check_call(
    f"./build/yabil_benchmarks --benchmark_out={results_output} --benchmark_out_format=json {filter_flag}",
    shell=True,
)

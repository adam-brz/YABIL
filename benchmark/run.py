#!/usr/bin/python

import os, sys
import platform
import subprocess
import shutil

results_output = "./build/results"

os.makedirs("build", exist_ok=True)

subprocess.check_call("conan export ../conan/recipe yabil/0.1@", shell=True)

if platform.system() == "Linux":
    profile = "clang"
    additional_flags = "-c tools.system.package_manager:mode=install"
    build_flags = "-DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang"
elif platform.system() == "Darwin":
    profile = "macos"
    additional_flags = ""
    build_flags = ""

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


filters = ["Addition", "Subtraction", "Multiplication", "Division"]

for f in filters:
    subprocess.check_call(
        f"./build/yabil_benchmarks --benchmark_repetitions=10 --benchmark_report_aggregates_only=true --benchmark_out={results_output}{f}.json --benchmark_out_format=json --benchmark_filter={f}",
        shell=True,
    )

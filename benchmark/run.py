#!/usr/bin/python

import subprocess

results_output = "./build/results.json"

subprocess.check_call("conan export ../conan/recipe", shell=True)

subprocess.check_call(
    "conan install .. -of=. -pr ../../conan/profiles/clang -s build_type=Release --build=missing",
    shell=True,
    cwd="build",
)

subprocess.check_call(
    "cmake --preset release -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
    shell=True,
)

subprocess.check_call("cmake --build ./build -j 12", shell=True)

subprocess.check_call(
    f"./build/yabil_benchmarks --benchmark_out={results_output} --benchmark_out_format=json --benchmark_filter='addition'",
    shell=True,
)

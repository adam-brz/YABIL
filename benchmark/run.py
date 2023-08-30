#!/usr/bin/python

import os
import platform
import subprocess
import shutil
import multiprocessing

subprocess.check_call("conan export ../conan/recipe yabil/0.1@", shell=True)

if platform.system() == "Linux":
    profile = "clang"
    additional_flags = "-c tools.system.package_manager:mode=install"
    build_flags = "-DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang"
elif platform.system() == "Darwin":
    profile = "macos"
    additional_flags = ""
    build_flags = ""


def build(benchmark_root_dir, conanfile_case, filters):
    build_dir = os.path.join(benchmark_root_dir, "build", conanfile_case)
    conanfile_path = os.path.join(benchmark_root_dir, "cases", conanfile_case) + ".txt"

    os.makedirs(build_dir, exist_ok=True)

    subprocess.check_call(
        f"conan install {conanfile_path} -of=. -pr {benchmark_root_dir}/../conan/profiles/{profile} -s build_type=Release --build=missing {additional_flags}",
        shell=True,
        cwd=build_dir,
    )

    shutil.copyfile(f"{build_dir}/CMakePresets.json", "CMakePresets.json")

    subprocess.check_call(
        f"cmake --preset release {build_flags} -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B {build_dir}",
        shell=True,
    )

    subprocess.check_call(
        f"cmake --build {build_dir} -j {multiprocessing.cpu_count() or 1}", shell=True
    )

    for f in filters:
        results_output = os.path.join(build_dir, f"results_{f.replace('/', '_')}.json")
        if "*" in f:
            results_output = os.path.join(build_dir, "results_all.json")
        subprocess.call(
            f"{build_dir}/yabil_benchmarks --benchmark_repetitions=10 --benchmark_report_aggregates_only=true --benchmark_out={results_output} --benchmark_out_format=json --benchmark_filter='{f}'",
            shell=True,
        )


root = os.path.abspath(os.path.dirname(__file__))
cases_filters = {
    # "uint32": ["YABIL"],
    # "uint16": ["YABIL"],
    # "normal_tbb": ["YABIL"],
    # "no_optimizations": ["YABIL", "openssl", "GMP"],
    # "normal": [".*"],
    "normal": ["YABIL_parallel_thread"],
}

for case, filters in cases_filters.items():
    build(root, case, filters)

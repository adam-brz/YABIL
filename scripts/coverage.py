import os
import glob
import shutil
import subprocess

from argparse import ArgumentParser
from build import conan_install, cmake_config, cmake_build


def generate_coverage_report(src_dir, binary_dir, lib_dir, output_dir):
    merge_raw_profiles(binary_dir)
    generate_html(src_dir, binary_dir, lib_dir, output_dir)


def merge_raw_profiles(binary_dir):
    raw_profiles = glob.glob(f"{binary_dir}/*.profraw")
    subprocess.check_call(
        f"llvm-profdata merge -sparse {' '.join(raw_profiles)} -o coverage.profdata",
        shell=True,
        cwd=binary_dir,
    )


def generate_html(src_dir, binary_dir, lib_dir, output_dir):
    source_files = glob.glob(f"{src_dir}/**/.cpp", recursive=True) + glob.glob(
        f"{src_dir}/**/.h", recursive=True
    )
    library_files = glob.glob(f"{lib_dir}/*.a")
    exec_files = list(filter(lambda f: f.find(".") == -1, glob.glob(f"{binary_dir}/*")))

    object_flags = ["-object " + file for file in (library_files + exec_files)]
    source_flags = ["-sources " + file for file in source_files]

    subprocess.check_call(
        f"llvm-cov show -format=html -output-dir={output_dir} -instr-profile coverage.profdata {' '.join(object_flags)} {' '.join(source_flags)}",
        shell=True,
        cwd=binary_dir,
    )


if __name__ == "__main__":
    source_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    build_type = "Debug"
    build_dir = f"{source_dir}/build/coverage"

    conan_install(source_dir, "clang", build_type, build_dir)
    cmake_config("coverage")
    cmake_build(build_dir, build_type)

    generate_coverage_report(
        source_dir,
        f"{build_dir}/bin",
        f"{build_dir}/lib",
        f"{build_dir}/coverage_report",
    )

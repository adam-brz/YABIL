import os
import glob
import multiprocessing
import subprocess

from build import conan_install, cmake_config, cmake_build
from argparse import ArgumentParser

def generate_coverage_report(llvm_profdata, llvm_cov):
    source_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    build_type = "Debug"
    build_dir = f"{source_dir}/build/coverage"

    conan_install(source_dir, "clang", build_type, build_dir)
    cmake_config("coverage")
    cmake_build(build_dir, build_type)

    subprocess.check_call(
        f"ctest -C Debug -j {multiprocessing.cpu_count()}",
        shell=True,
        cwd=build_dir,
    )

    binary_dir = f"{build_dir}/bin"
    lib_dir = f"{build_dir}/lib"
    output_dir = f"{build_dir}/coverage_report"

    merge_raw_profiles(llvm_profdata, binary_dir)
    generate_html(llvm_cov, source_dir, binary_dir, lib_dir, output_dir)


def merge_raw_profiles(llvm_profdata, binary_dir):
    raw_profiles = glob.glob(f"{binary_dir}/*.profraw")
    subprocess.check_call(
        f"{llvm_profdata} merge -sparse {' '.join(raw_profiles)} -o coverage.profdata",
        shell=True,
        cwd=binary_dir,
    )


def generate_html(llvm_cov, src_dir, binary_dir, lib_dir, output_dir):
    source_files = glob.glob(f"{src_dir}/**/.cpp", recursive=True) + glob.glob(
        f"{src_dir}/**/.h", recursive=True
    )
    library_files = glob.glob(f"{lib_dir}/*.a")
    exec_files = list(filter(lambda f: f.find(".") == -1, glob.glob(f"{binary_dir}/*")))

    object_flags = ["-object " + file for file in (library_files + exec_files)]
    source_flags = ["-sources " + file for file in source_files]

    subprocess.check_call(
        f"{llvm_cov} show -format=html -output-dir={output_dir} -instr-profile coverage.profdata {' '.join(object_flags)} {' '.join(source_flags)}",
        shell=True,
        cwd=binary_dir,
    )

    subprocess.check_call(
        f"{llvm_cov} report -show-region-summary=false -show-functions=false -instr-profile coverage.profdata {' '.join(object_flags)} {' '.join(source_flags)}",
        shell=True,
        cwd=binary_dir,
    )


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument(
        "-p",
        "--llvm_profdata",
        required=False,
        default="llvm-profdata",
        help="llvm-profdata executable path",
    )
    parser.add_argument(
        "-c",
        "--llvm_cov",
        required=False,
        default="llvm-cov",
        help="llvm-cov executable path",
    )
    args = parser.parse_args()
    generate_coverage_report(args.llvm_profdata, args.llvm_cov)


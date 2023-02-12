import os
import subprocess
import multiprocessing
from argparse import ArgumentParser


def build(build_type, preset_name):
    full_preset_name = f"{preset_name}_{build_type.lower()}"
    preset_dir = f"build/{full_preset_name}"
    source_dir = os.path.dirname(os.path.abspath(__file__))

    os.makedirs(preset_dir, exist_ok=True)

    subprocess.check_call(
        f"conan install {source_dir}/conan -pr:b={source_dir}/conan/profiles/{preset_name} "
        f"-s build_type={build_type} --build=missing",
        shell=True,
        cwd=preset_dir,
    )
    subprocess.check_call(
        f"cmake --preset {full_preset_name}",
        shell=True,
    )
    subprocess.check_call(
        f"cmake --build {preset_dir} -j {multiprocessing.cpu_count()} --config {build_type}",
        shell=True,
    )


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument(
        "-p",
        "--preset",
        required=True,
        choices=["vs", "clang", "gcc"],
        help="CMake preset name to use",
    )
    parser.add_argument(
        "-b",
        "--build_type",
        default="Release",
        choices=["Release", "Debug"],
        help="CMake build type",
    )
    args = parser.parse_args()
    build(args.build_type, args.preset)

import subprocess
import glob

import pandas as pd
import matplotlib.pyplot as plt

YABIL_VERSION = 0.1


def show_csv_data(filename):
    df = pd.read_csv(filename, header=None, names=["data_size", "time"], sep=",")
    plt.loglog(df.data_size, df.time)


if __name__ == "__main__":
    subprocess.check_call(
        "CC=clang CXX=clang++ conan create -pr ../../conan/profiles/clang -s build_type=Release ../../conan/recipe",
        shell=True,
        cwd="performance_stats",
    )
    subprocess.check_call(
        f"CC=clang CXX=clang++ conan test -pr ../../conan/profiles/clang -s build_type=Release . yabil/{YABIL_VERSION} --build=missing",
        shell=True,
        cwd="performance_stats",
    )

    csv_results = glob.glob("performance_stats/build/**/*.csv", recursive=True)

    for csv in csv_results:
        show_csv_data(csv)
    plt.show()

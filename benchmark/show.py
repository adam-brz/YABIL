#!/usr/bin/python

import json
import os

import matplotlib.pyplot as plt

results_output = "./build/results.json"

with open(results_output, "r") as results_file:
    results = json.load(results_file)

results_by_function = {}
for benchmark in results["benchmarks"]:
    name, iter = benchmark["name"].rsplit("/", 1)
    test_type, library = name.split("_", 1)
    time = benchmark["cpu_time"]

    if test_type not in results_by_function:
        results_by_function[test_type] = {}
    if library not in results_by_function[test_type]:
        results_by_function[test_type][library] = {}
    results_by_function[test_type][library][iter] = time

os.makedirs("build/res", exist_ok=True)

for func in results_by_function:
    plt.figure()
    plt.title(func.capitalize())
    for lib in results_by_function[func]:
        x, y = (
            [int(v) for v in results_by_function[func][lib].keys()],
            results_by_function[func][lib].values(),
        )
        plt.plot(x, y)
    legend_entries = list(results_by_function[func].keys())
    plt.legend([entry.replace("YABIL", "Self implementation").replace("_", " ") for entry in legend_entries])
    plt.grid()
    plt.xlabel("digits [n]")
    plt.ylabel("time [ns]")
    plt.savefig(f"build/res/{func}.png")

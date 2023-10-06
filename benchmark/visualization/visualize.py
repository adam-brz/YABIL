#!/usr/bin/env python
# coding: utf-8

# In[16]:


import json
import os

import uuid

import matplotlib.pyplot as plt
import numpy as np


# In[17]:


data_dir = "generated/raw"


def load_case(case_name):
    case_dir = os.path.join(data_dir, case_name)
    results = {}
    for data_filename in os.listdir(case_dir):
        with open(os.path.join(case_dir, data_filename), "r") as data_file:
            results[os.path.splitext(data_filename)[0]] = json.load(data_file)
    return results


cases = {}
for case in os.listdir(data_dir):
    print(case)
    cases[case] = load_case(case)


# In[18]:


import re

PARALLEL_PATTERN = re.compile("YABIL_parallel_thread_(\d+)_threads")


def tr(text):
    translation_map = {
        "YABIL": "własne",
        "YABIL_parallel": "własne [12 wątków]",
        "boost": "Boost",
        "openssl": "OpenSSL",
        "python": "Python",
        "Addition": "Dodawanie",
        "Division": "Dzielenie",
        "Multiplication": "Mnożenie",
        "Subtraction": "Odejmowanie",
        "YABIL_parallel_thread_1_threads": "własne [1 wątek]",
        "YABIL_parallel_thread_2_threads": "własne [2 wątki]",
        "YABIL_parallel_thread_3_threads": "własne [3 wątki]",
        "YABIL_parallel_thread_4_threads": "własne [4 wątki]",
        "YABIL_parallel_6_threads": "własne [6 wątków]",
    }

    text = text.replace("_big", "")
    if text in translation_map:
        return translation_map[text]
    if match := PARALLEL_PATTERN.match(text):
        return f"własne [{match[1]} wątków]"
    return text


# In[19]:


import numpy as np
import seaborn as sns
import colorcet as cc


def load(data):
    plot_data = {}
    for entry in data:
        name = entry["name"]
        parts = name.replace("/real_time", "").split("/")

        if len(parts) == 3:
            operation, library, n_aggregate = parts
            n, aggregate = n_aggregate.split("_")
        if len(parts) == 4:
            operation, library, n, threads_aggregate = parts
            threads, aggregate = threads_aggregate.split("_")
            threads = int(threads) + 1
            library = f"{library}_{threads}_threads"

        if aggregate == "mean":
            cpu_time = entry["real_time"]
            if operation not in plot_data:
                plot_data[operation] = {}
            if library not in plot_data[operation]:
                plot_data[operation][library] = {}
            plot_data[operation][library][int(n)] = cpu_time
    return plot_data


def get_color(data_label):
    colors = {
        "YABIL": "#4363d8",
        "YABIL_big": "#4363d8",
        "YABIL_parallel": "#000075",
        "YABIL_big_parallel": "#000075",
        "FLINT": "#6929c4",
        "GMP": "#f58231",
        "openssl": "#204200",
        "python": "#a6194B",
        "boost": "#3cb44b",
        "uint16": "#42d4f4",
        "uint32": "#911eb4",
        "uint64": "#4363d8",
        "uint64 [Bez AVX]": "#469990",
        "tbb": "red",
        "tbb2": "purple",
    }

    palette = sns.color_palette(cc.glasbey_dark, n_colors=14)
    for i in range(1, 13):
        colors[f"YABIL_parallel_thread_{i}_threads"] = palette[i - 1]
        colors[f"YABIL_parallel_{i}_threads"] = palette[i - 1]

    data_label = data_label.replace("_big", "")
    return colors[data_label]


def plot_internal(operation_name, operation_data, exclude=[], yscale="linear"):
    for lib_name, lib_data in operation_data.items():
        if any((excluded in lib_name) for excluded in exclude):
            continue
        x, y = np.array(list(lib_data.keys())), np.array(list(lib_data.values()))
        plt.plot(
            x / 1000,
            y / 1000,
            "--",
            marker=".",
            markersize=5,
            color=get_color(lib_name),
        )

    plt.legend(
        [
            " ".join(tr(label).split("_"))
            for label in operation_data.keys()
            if label not in exclude
        ]
    )
    plt.title(tr(operation_name))
    plt.xlabel("Rozmiar danych [kilobity]")
    plt.ylabel("Czas operacji [μs]")
    plt.grid()
    plt.xlim(0, 1e3)
    plt.yscale(yscale)
    if yscale == "linear":
        plt.ylim(0)
        plt.gca().ticklabel_format(style="plain")


def save(operation_name):
    return
    # plt.savefig(
    # f"generated/{operation_name}_{uuid.uuid4().hex}.png",
    # bbox_inches="tight",
    # dpi=300,
    # )


def plot_common(operation_name, operation_data, exclude=[], yscale="linear"):
    plot_internal(operation_name, operation_data, exclude, yscale)
    save(operation_name)
    plt.show()


# In[20]:


# Optimized libraries comparison

all_data = cases["normal"]["results_all"]["benchmarks"]
data = load(all_data)

for operation, operation_data in data.items():
    plot_common(
        operation,
        operation_data,
        ["BIGINT_mattmccutchen", "YABIL_parallel", "_big", "FLINT"],
    )


# In[21]:


# Optimized libraries comparison (only multiplication)

# all_data = cases["normal"]["results_all"]["benchmarks"]
# data = load(all_data)
# plot_common("Multiplication", data["Multiplication"], ["BIGINT_mattmccutchen", "python", "YABIL"])


# # -----------------------------------------

# In[22]:


# C++ only libraries comparison

gmp_data = load(cases["no_optimizations"]["results_GMP"]["benchmarks"])
openssl_data = load(cases["no_optimizations"]["results_openssl"]["benchmarks"])
all_data = load(cases["normal"]["results_all"]["benchmarks"])

normal = {}
for op, data in all_data.items():
    if op not in normal:
        normal[op] = {}
    normal[op]["YABIL"] = data["YABIL"]

for k in gmp_data:
    normal[k].update(gmp_data[k])
    normal[k].update(openssl_data[k])

for operation, operation_data in normal.items():
    plot_common(operation, operation_data, ["GMP_big"])


# In[23]:


# All YABIL options


def rearrange(data, new="YABIL", old="YABIL"):
    res = {}
    for op, lib_data in data.items():
        if op not in res:
            res[op] = {}
        res[op][new] = lib_data[old]
        # if op != "Addition" and "YABIL_parallel" in lib_data:
        #     res[op][f"{new}_parallel"] = lib_data["YABIL_parallel"]
    return res


all_data = rearrange(load(cases["normal"]["results_all"]["benchmarks"]), "uint64")
no_avx = rearrange(
    load(cases["no_optimizations"]["results_YABIL"]["benchmarks"]), "uint64 [Bez AVX]"
)
# tbb = rearrange(load(cases["normal_tbb"]["results_YABIL"]["benchmarks"]), "uint64 [TBB]")
uint16 = rearrange(load(cases["uint16"]["results_YABIL"]["benchmarks"]), "uint16")
uint32 = rearrange(load(cases["uint32"]["results_YABIL"]["benchmarks"]), "uint32")

for k in all_data:
    all_data[k].update(no_avx[k])
    # all_data[k].update(tbb[k])
    all_data[k].update(uint16[k])
    all_data[k].update(uint32[k])

for operation, operation_data in all_data.items():
    plot_common(operation, operation_data)


# In[24]:


all_data = load(cases["parallel"]["results_YABIL_parallel_thread"]["benchmarks"])
# mul_12_threads = {"YABIL_parallel_thread_12_threads": load(cases["normal"]["results_all"]["benchmarks"])["Multiplication"]["YABIL_parallel"]}
mul_1_threads = {
    "Multiplication": {
        "YABIL_parallel_thread_1_threads": load(
            cases["normal"]["results_all"]["benchmarks"]
        )["Multiplication"]["YABIL"]
    }
}

mul_1_threads["Multiplication"].update(all_data["Multiplication"])
# mul_1_threads["Multiplication"].update(mul_12_threads)

for operation, operation_data in mul_1_threads.items():
    plot_common("Mnożenie wielowątkowe", operation_data)


# In[25]:


all_data = load(cases["parallel"]["results_YABIL_parallel_thread"]["benchmarks"])
mul_12_threads = {
    "YABIL_parallel_thread_12_threads": load(
        cases["normal"]["results_all"]["benchmarks"]
    )["Division"]["YABIL_parallel"]
}
mul_1_threads = {
    "Division": {
        "YABIL_parallel_thread_1_threads": load(
            cases["normal"]["results_all"]["benchmarks"]
        )["Division"]["YABIL"]
    }
}

mul_1_threads["Division"].update(all_data["Division"])
mul_1_threads["Division"].update(mul_12_threads)

for operation, operation_data in mul_1_threads.items():
    plot_common("Dzielenie wielowątkowe", operation_data)


# In[26]:


all_data = load(cases["parallel"]["results_YABIL_parallel_thread"]["benchmarks"])
mul_12_threads = {
    "YABIL_parallel_thread_12_threads": load(
        cases["normal"]["results_all"]["benchmarks"]
    )["Addition"]["YABIL_parallel"]
}
mul_1_threads = {
    "Addition": {
        "YABIL_parallel_thread_1_threads": load(
            cases["normal"]["results_all"]["benchmarks"]
        )["Addition"]["YABIL"]
    }
}

mul_1_threads["Addition"].update(all_data["Addition"])
mul_1_threads["Addition"].update(mul_12_threads)

for operation, operation_data in mul_1_threads.items():
    plot_common("Dodawanie wielowątkowe", operation_data)


# In[27]:


# parallel_6 = load(cases["parallel"]["results_YABIL_parallel_thread"]["benchmarks"])["Addition"]["YABIL_parallel_thread_6_threads"]
# seq = load(cases["normal"]["results_all"]["benchmarks"])["Addition"]["YABIL"]

# del parallel_6[0]

# limit = 2.5e6

# p6_x, p6_y = np.array(list(parallel_6.keys())), np.array(list(parallel_6.values()))
# seq_x, seq_y = np.array(list(seq.keys())), np.array(list(seq.values()))

# plt.plot(p6_x/1000, p6_y/1000, '.', markersize=5, color=get_color("YABIL_parallel_thread_6_threads"))
# plt.plot(seq_x/1000, seq_y/1000, '.', markersize=5, color=get_color("YABIL_parallel_thread_1_threads"))

# lin_p6 = np.polyfit(p6_x, p6_y, 1)
# lin_seq = np.polyfit(seq_x, seq_y, 1)

# x = np.arange(0, limit)
# y_p6 = np.polyval(lin_p6, x)
# y_seq = np.polyval(lin_seq, x)

# plt.plot(x/1000, y_p6/1000, "--")
# plt.plot(x/1000, y_seq/1000, "--")

# intersection_x = (lin_seq[1] - lin_p6[1]) / (lin_p6[0] - lin_seq[0])
# print("interseciton point:", intersection_x)

# plt.legend(["6 wątków", "1 wątek", "6 wątków [aproksymacja f. liniową]", "1 wątek [aproksymacja f. liniową]", "punkt przecięcia"])
# plt.title("Ekstrapolacja czasu dodawania")
# plt.xlabel("Rozmiar danych [kilobity]")
# plt.ylabel("Czas operacji [μs]")
# plt.grid()
# plt.xlim(0, limit/1000)
# plt.ylim(0)
# plt.gca().ticklabel_format(style="plain")

# plt.savefig(
#     f"generated/AdditionExtrapolated.png",
#     bbox_inches="tight",
#     dpi=300,
# )

# plt.show()


# In[28]:


all_data = load(cases["normal"]["results_all"]["benchmarks"])

operation_limits = {"Addition": 1.8e7, "Division": 4e6, "Multiplication": 4e6}

for operation, operation_data in all_data.items():
    plot_internal(
        operation, {k: v for k, v in operation_data.items() if "_big" in k}
    )  # operation_data
    plt.xlim(0, operation_limits[operation] / 1000)
    # plt.ylim(0, 1400)
    save(operation)
    plt.show()

operation, operation_data = "Addition", all_data["Addition"]
plot_internal(
    operation, {k: v for k, v in operation_data.items() if "_big" in k}
)  # operation_data
plt.xlim(0, 6e3)
plt.ylim(0, 400)
save(operation)
plt.show()

operation, operation_data = "Multiplication", all_data["Multiplication"]
plot_internal(
    operation, {k: v for k, v in operation_data.items() if "_big" in k}
)  # operation_data
# plt.xlim(0, 4e3)
plt.ylim(0, 40000)
save(operation)
plt.show()

operation, operation_data = "Division", all_data["Division"]
plot_internal(
    operation, {k: v for k, v in operation_data.items() if "_big" in k}
)  # operation_data
# plt.xlim(0, 4e3)
plt.ylim(0, 80000)
save(operation)
plt.show()

# In[29]:
# TBB
all_data = load(cases["normal"]["results_all"]["benchmarks"])
tbb = rearrange(
    load(cases["normal_tbb"]["results_YABIL_parallel"]["benchmarks"]),
    "tbb",
    "YABIL_parallel",
)
tbb2 = rearrange(
    load(cases["normal_tbb"]["results_YABIL_parallel2"]["benchmarks"]),
    "tbb2",
    "YABIL_parallel",
)

for k in all_data:
    all_data[k].update(tbb[k])
    all_data[k].update(tbb2[k])

for operation, operation_data in all_data.items():
    plot_internal(operation, operation_data)
    plt.ylim(0, 100)
    plt.show()

# %%

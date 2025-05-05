#!/usr/bin/env python3
import csv
from collections import defaultdict
import matplotlib.pyplot as plt
from math import log10
import numpy as np
import os
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import LinearRegression

IMG_DIR = "img"
FILES = ["results_fl_z1.csv", "results_cl_z1.csv", "results_cl_z2.csv"]

FL_VALUES = ["Cost", "Time [s]"]
CL_VALUES = ["Clusters"] + FL_VALUES
SOLUTION_COLORS = {
    "Mettu-Plaxton": "red",
    "K-means++ (scikit-learn)": "red",
    "Grid hashing": "blue",
    "Face hashing": "green"
}
SOLUTION_MARKER = {
    "Mettu-Plaxton": "o",
    "K-means++ (scikit-learn)": "o",
    "Grid hashing": "^",
    "Face hashing": "v"
}

def get_color(args: list[str]):
    if len(args) == 0:
        return "r"
    elif args[0] == "grid_hashing":
        return "b"
    elif args[0] == "face_hashing":
        return "g"

def plot_instance(title: str, values):
    solutions = defaultdict(list)
    for val in values:
        solutions[val[1]].append(val)

    for val_number, val_name in enumerate(FL_VALUES if len(values[0]) == 4 else CL_VALUES):
        fig, ax = plt.subplots()

        for sol_name, sol_val in reversed(solutions.items()):
            xs = [v[0] for v in sol_val]
            ys = [v[val_number+2] for v in sol_val]
            ax.plot(
                xs, ys, SOLUTION_MARKER[sol_name] + "-",
                c=SOLUTION_COLORS[sol_name],
                label=sol_name,
            )

            if val_name == "Time [s]":
                func_xs = np.logspace(log10(min(xs)), log10(max(xs)), 100)
                if sol_name == "Mettu-Plaxton":
                    a = 0.00000016*d
                    b = 0.000000005*d
                    c = 0.005
                    # ax.plot(
                    #     func_xs, [a*x**2 + b*x+c for x in func_xs],
                    #     "--",
                    #     c=SOLUTION_COLORS[sol_name],
                    #     label=f"{a:.2e}*n^2+{b:.2e}*n+{c:.2e}"
                    # )
                else:
                    x_scaler = StandardScaler()
                    y_scaler = StandardScaler()
                    x_scaled = x_scaler.fit_transform(np.array(xs).reshape(-1, 1))
                    y_scaled = y_scaler.fit_transform(np.array(ys).reshape(-1, 1))
                    lr = LinearRegression()
                    lr.fit(x_scaled, y_scaled)

                    mu_x = x_scaler.mean_[0]
                    sigma_x = x_scaler.scale_[0]
                    mu_y = y_scaler.mean_[0]
                    sigma_y = y_scaler.scale_[0]
                    m = lr.coef_[0][0]
                    c = lr.intercept_[0]
                    a = (sigma_y / sigma_x) * m
                    b = sigma_y * c + mu_y - (sigma_y * m * mu_x / sigma_x)

                    # ax.plot(
                    #     func_xs, [x*a + b for x in func_xs],
                    #     "--",
                    #     c=SOLUTION_COLORS[sol_name],
                    #     label=f"{a:.2e}*n+{b:.2e}"
                    # )

        plt.title(title)
        plt.xlabel("Input size (n)")
        plt.ylabel(val_name)
        ax.legend()

        ax.set_xscale("log")
        ax.set_yscale("log")

        REPLACE = {" ": "_", "(": "", ")": "", ",": "", "=": ""}
        figname = "".join(
            REPLACE[l] if l in REPLACE else l
            for l in title.replace("(", val_name + " ").lower().replace(" [s]", "")
        ) + ".svg"
        plt.savefig(os.path.join(IMG_DIR, figname))
        plt.close()


def plot_file(filename: str):
    global d
    plot_name = "Facility location" if "fl" in filename else "Clustering"
    plot_name += f" (d=X, z={filename[-5]})"

    generated_dataset = defaultdict(list)
    with open(filename) as f:
        reader = csv.reader(f)

        for line in reader:
            inp, solution, args, *params = line
            args = args.split()

            if len(args) == 2:
                if args[0] == "grid_hashing":
                    solution = "Grid hashing"
                elif args[0] == "face_hashing":
                    solution = "Face hashing"
                else:
                    raise ValueError(f"Unrecognized argument: {args[0]}")
            elif solution.startswith("mettu_plaxton"):
                solution = "Mettu-Plaxton"
            elif solution.startswith("scikit"):
                solution = "K-means++ (scikit-learn)"
            else:
                raise ValueError("Unknown solution")

            if inp.startswith("gen"):
                _, n, d, _ = inp.replace(".", "_").split("_")
                n = int(n[1:])
                d = int(d[1:])
                generated_dataset[d].append((n, solution, *map(float, params)))
    
    for d, d_values in generated_dataset.items():
        plot_instance(plot_name.replace("d=X", f"d={d}"), d_values)

if __name__ == "__main__":
    os.makedirs(IMG_DIR, exist_ok=True)
    for file in FILES:
        plot_file(file)



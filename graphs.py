#!/usr/bin/env python3
import csv
from collections import defaultdict
import matplotlib.pyplot as plt
from math import log10
import numpy as np
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import LinearRegression

FILES = ["results_fl_z1.csv"]

VALUES = [("Cost", 1), ("Time [s]", 0.01)]
SOLUTION_COLORS = {
    "Mettu-Plaxton": "red",
    "K-means++ (scikit-learn)": "red",
    "Grid hashing": "blue",
    "Face hashing": "green"
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

    for val_number, (val_name, val_min) in enumerate(VALUES):
        fig, ax = plt.subplots()
        
        for sol_name, sol_val in solutions.items():
            xs = [v[0] for v in sol_val]
            ys = [v[val_number+2] for v in sol_val]
            ax.plot(
                xs, ys, "x",
                c=SOLUTION_COLORS[sol_name],
                label=sol_name,
                markersize=12,
            )

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

            func_xs = np.logspace(log10(min(xs)), log10(max(xs)), 1000)
            ax.plot(
                func_xs, [a*x+b for x in func_xs],
                "--",
                c=SOLUTION_COLORS[sol_name],
                alpha=0.7,
                label=f"{a}*n+{b}",
            )

        plt.title(title)
        plt.xlabel("Input size (N)")
        plt.ylabel(val_name)
        ax.legend()

        ax.set_xscale("log")
        ax.set_yscale("log")

        plt.show()

            
def plot_file(filename: str):
    plot_name = "Facility location" if "fl" in filename else "Clustering"
    plot_name += f" (D=X, Z={filename[-5]})"

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
                print(solution)
                raise ValueError("Unknown solution")

            if inp.startswith("gen"):
                _, n, d, _ = inp.replace(".", "_").split("_")
                n = int(n[1:])
                d = int(d[1:])
                generated_dataset[d].append((n, solution, *map(float, params)))
    
    for d, d_values in generated_dataset.items():
        plot_instance(plot_name.replace("D=X", f"D={d}"), d_values)

if __name__ == "__main__":
    for file in FILES:
        plot_file(file)



#!/usr/bin/env python3
from random import seed, shuffle
import argparse
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D
import numpy as np
from sklearn.decomposition import PCA


def groups(inp, out, monocolor):
    if len(out) == 0 or monocolor:
        return [0] * len(inp)
    else:
        return [
            min(range(len(out)), key=lambda i: np.linalg.norm(p - out[i]))
            for p in inp
        ]


def main(args):
    with open(args.input) as inp:
        num_points, _, _ = map(int, inp.readline().split())

        in_points = [
            list(map(float, inp.readline().split()))
            for _ in range(num_points)
        ]

    if args.output is not None:
        with open(args.output) as out:
            out_points = [
                list(map(float, line.split()))
                for line in out.readlines()
                if line.strip()
            ]
            out_points.sort()
    else:
        out_points = []

    data = np.array(in_points + out_points)
    in_points = np.array(in_points)
    out_points = np.array(out_points)

    pca = PCA(n_components=2, random_state=42)
    reduced = pca.fit_transform(data)
    fig, ax = plt.subplots()
    
    cmap = plt.get_cmap("viridis")

    # Plot input
    INP_LABEL = "input point"
    if len(out_points) == 0:
        inp_legend = []
        ax.scatter(
            reduced[:len(in_points), 0],
            reduced[:len(in_points), 1],
            color="blue",
            label=INP_LABEL
        )
    else:
        ax.scatter(
            reduced[:len(in_points), 0],
            reduced[:len(in_points), 1],
            c=[
                cmap(0.4+g/len(out_points)/(1 if len(out_points) < 5 else 1.5))
                for g in groups(in_points, out_points, args.monocolor)
            ]
        )
        # Create legend marker manually
        inp_legend = [Line2D([0], [0], marker='o', markerfacecolor='white', markeredgecolor='black', linestyle='None', label=INP_LABEL)]

    # Plot centers
    if args.output is not None:
        ax.scatter(
            reduced[len(in_points):, 0],
            reduced[len(in_points):, 1],
            c=[0]*len(out_points),
            marker="+",
            label="opened facility" if "mettu" in args.output or "facility" in args.output else "cluster center"  # XXX: Ugly hack
        )

    plt.title(args.title)
    plt.xlabel('Principal component 1')
    plt.ylabel('Principal component 2')

    ax.legend(handles=inp_legend + ax.get_legend_handles_labels()[0])
    
    if args.save is None:
        plt.show()
    else:
        plt.savefig(args.save)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Visualize data using PCA.")
    parser.add_argument("input", help="Path to the input file.")
    parser.add_argument("output", nargs='?', default=None, help="Path to the output.")
    parser.add_argument("--monocolor", action="store_true", help="Color the points in the input with the same color.")
    parser.add_argument("--title", default="", help="Title of the plot")
    parser.add_argument("--save", help="Path to file to save the plot")
    args = parser.parse_args()
    main(args)

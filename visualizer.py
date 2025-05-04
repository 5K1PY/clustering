#!/usr/bin/env python3
from random import seed, shuffle
import argparse
import matplotlib.pyplot as plt
import numpy as np
from sklearn.decomposition import PCA


def labels(inp, out, monocolor):
    if len(out) == 0 or monocolor:
        return ["blue"] * len(inp) + ["red"] * len(out)
    else:
        return [
            min(range(len(out)), key=lambda i: np.linalg.norm(p - out[i]))
            for p in inp
        ] + [-1 if len(out) < 5 else -0.5*len(out)] * len(out)


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
    ax.scatter(
        reduced[:, 0],
        reduced[:, 1],
        c=labels(in_points, out_points, args.monocolor)
    )

    plt.title(args.title)
    plt.xlabel('Principal component 1')
    plt.ylabel('Principal component 2')
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

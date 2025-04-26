#!/usr/bin/env python3
from random import seed, shuffle
import argparse
import matplotlib.pyplot as plt
import numpy as np
from sklearn.decomposition import PCA


def labels(inp, out, monocolor):
    if monocolor:
        return ["blue"] * len(inp) + ["red"] * len(out)
    else:
        return [
            min(range(len(out)), key=lambda i: np.linalg.norm(p - out[i]))
            for p in inp
        ] + [-1 if len(out) < 5 else -0.5*len(out)] * len(out)


def main(inp_path: str, out_path: str | None, monocolor: bool):
    with open(inp_path) as inp:
        num_points, _, _ = map(int, inp.readline().split())

        in_points = [
            list(map(float, inp.readline().split()))
            for _ in range(num_points)
        ]

    if out_path is not None:
        with open(out_path) as out:
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
        c=labels(in_points, out_points, monocolor)
    )

    plt.show()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Visualize data using t-SNE.")
    parser.add_argument("input", help="Path to the input file.")
    parser.add_argument("output", nargs='?', default=None, help="Path to the output.")
    parser.add_argument("--monocolor", action="store_true", help="Color the points in the input with the same color.")
    args = parser.parse_args()
    main(args.input, args.output, args.monocolor)

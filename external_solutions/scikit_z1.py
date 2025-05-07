#!/usr/bin/env python3
import argparse
import numpy as np
from sklearn_extra.cluster import KMedoids

def main(method):
    num_points, dimension, k = map(int, input().split())

    points = [
        list(map(float, input().split()))
        for _ in range(num_points)
    ]

    data = np.array(points)

    kmediods = KMedoids(n_clusters=k, method=method, random_state=42)
    kmediods.fit(data)

    for center in kmediods.cluster_centers_:
        print(" ".join(map(str, center)))

if __name__ == "__main__":
    parser = argparse.ArgumentParser(prog='scikit_z1', description='K-medians solution based on K-medoids from scikit-learn-extra')
    parser.add_argument("method", choices=["alternate", "pam"])
    args = parser.parse_args()
    main(args.method)

#!/usr/bin/env python3
import numpy as np
from sklearn_extra.cluster import KMedoids

def main():
    num_points, dimension, k = map(int, input().split())

    points = [
        list(map(float, input().split()))
        for _ in range(num_points)
    ]

    data = np.array(points)

    kmediods = KMedoids(n_clusters=k, metric="manhattan", random_state=42)
    kmediods.fit(data)

    for center in kmediods.cluster_centers_:
        print(" ".join(map(str, center)))

if __name__ == "__main__":
    main()

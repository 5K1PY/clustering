#!/usr/bin/env python3
import numpy as np
from sklearn.cluster import KMeans

def main():
    num_points, dimension, k = map(int, input().split())

    points = [
        list(map(float, input().split()))
        for _ in range(num_points)
    ]

    data = np.array(points)

    kmeans = KMeans(n_clusters=k, random_state=42)
    kmeans.fit(data)

    for center in kmeans.cluster_centers_:
        print(" ".join(map(str, center)))

if __name__ == "__main__":
    main()

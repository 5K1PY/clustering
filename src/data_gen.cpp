#include <algorithm>
#include <iostream>
#include <random>

#include "random.h"
#include "points.h"

using namespace std;

typedef unsigned long long ull;


point rand_point(int dim) {
    point p(dim);
    for (int i=0; i<dim; i++) {
        p[i] = randRange((ull) 0, (ull) numeric_limits<ull>::max());
    }
    return p;
}

point rand_shift(int dim) {
    point p(dim);
    for (int i=0; i<dim; i++) {
        p[i] = randNormal((ull) 0, (ull) 1e5);
    }
    return p;
}

vector<point> gen_random(int n, int dim) {
    vector<point> points(n, point(dim));
    for (int i=0; i<n; i++) {
        points[i] = rand_point(dim);
    }
    return points;
}

vector<point> gen_clusters(int n, int dim) {
    int cluster_count = sqrt(n);
    int free_points = sqrt(n); 

    int all_points_sz = 0;
    vector<point> all_points(n, point(dim));

    vector<point> centers(cluster_count, point(dim));
    for (int i=0; i<cluster_count; i++) {
        centers[i] = all_points[all_points_sz++] = rand_point(dim);
    }

    for (int i=0; i<free_points; i++) {
        all_points[all_points_sz++] = rand_point(dim);
    }

    while (all_points_sz < n) {
        int cluster = randRange(0, cluster_count-1);
        all_points[all_points_sz++] = centers[cluster] + rand_shift(dim); 
    }

    shuffle(all_points.begin(), all_points.end(), rng);
    return all_points;
}

int main() {
    int dimension, n;
    cin >> n >> dimension;

    vector<point> points = gen_clusters(n, dimension);

    cout << n << " " << dimension << "\n";
    for (auto p: points) {
        for (int i=0; i<dimension; i++) {
            cout << p[i] << (i+1 < dimension ? " " : "\n");
        }
    }
}

#include <assert.h>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>

#include "lib/random.hpp"
#include "lib/points.hpp"

constexpr ull MAX_COORD = 1e17;
constexpr ull CLUSTER_RADIUS = 1e15;

point rand_point(int dim) {
    point p(dim);
    for (int i=0; i<dim; i++) {
        p[i] = randRange<ull>(0ULL, MAX_COORD);
    }
    return p;
}

point rand_cluster_center(int dim) {
    point p(dim);
    for (int i=0; i<dim; i++) {
        // Restrict range to generate points within [0, MAX_COORD]
        p[i] = randRange<ull>(10*CLUSTER_RADIUS, MAX_COORD-10*CLUSTER_RADIUS);
    }
    return p;
}

point rand_shift(int dim) {
    point p(dim);
    for (int i=0; i<dim; i++) {
        p[i] = randNormal<ll>(0LL, CLUSTER_RADIUS);
    }
    return p;
}

std::vector<point> gen_random(int n, int dim) {
    std::vector<point> points(n, point(dim));
    for (int i=0; i<n; i++) {
        points[i] = rand_point(dim);
    }
    return points;
}

std::vector<point> gen_clusters(int n, int dim) {
    int cluster_count = sqrt(n);
    int free_points = sqrt(n); 

    int all_points_sz = 0;
    std::vector<point> all_points(n, point(dim));

    std::vector<point> centers(cluster_count, point(dim));
    for (int i=0; i<cluster_count; i++) {
        centers[i] = all_points[all_points_sz++] = rand_cluster_center(dim);
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
    double facility_cost;
    std::cin >> n >> dimension >> facility_cost;

    std::vector<point> points = gen_clusters(n, dimension);

    std::cout << n << " " << dimension << " " << facility_cost << "\n";
    std::cout << std::fixed << std::setprecision(10);
    for (auto p: points) {
        for (int i=0; i<dimension; i++) {
            // Theoretically it's possible to hit outside of this range
            // but the even hitting enough shift of has chance of Phi(10) which is basically zero
            assert(0 <= p[i] && p[i] <= (ll) MAX_COORD);
            std::cout << ((double) p[i] / scale) << (i+1 < dimension ? " " : "\n");
        }
    }
}

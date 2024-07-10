#pragma once

#include <queue>
#include <unordered_set>
#include <unordered_map>

#include "hashing.h"
#include "points.h"
#include "r_p.h"

using namespace std;

template<typename T>
struct Composable {
    T empty_value;
    virtual T evaluate(const point& p) = 0;
    virtual T compose(T val1, T val2) = 0;
};

struct Size : Composable<int> {
    int empty_value = 0;
    virtual int evaluate(const point& p) {
        return 1;
    }
    virtual int compose(int val1, int val2) {
        return val1 + val2;
    }
};

const int CELL_SIZE = 1e5;

template<typename T>
vector<T> eval_composable(int dim, vector<tagged_point>& points, ull r, Composable<T> f) {
    GridHashing hashing_scheme(dim, CELL_SIZE);

    for (auto p: points) {
        p.hash = hashing_scheme.hash(p);
    }

    unordered_map<ull, T> bucket_f;
    for (auto p: points) {
        if (bucket_f.find(p.hash) == bucket_f.end())
            bucket_f[p.hash] = f.empty_value;
        bucket_f[p.hash] = f.compose(bucket_f[p.hash], f.evaluate(p));
    }

    vector<T> proximity_points(points.size(), f.empty_value);
    for (int points_i=0; points_i<points.size(); points_i++) {
        queue<point> neighborhood;
        neighborhood.push(points[points_i]);
        unordered_set<ull> found_cells;

        while (neighborhood.size()) {
            point p = neighborhood.back(); neighborhood.pop();
            ull hash = hashing_scheme.hash(p);

            if (found_cells.count(hash))
                continue;

            proximity_points[points_i] = f.compose(
                proximity_points[points_i],
                bucket_f[hash]
            );

            for (int i=0; i<dim; i++) {
                point q = p;
                q[i] += CELL_SIZE;
                if (hashing_scheme.bucket_sphere_intersect(points[i], r, q))
                    neighborhood.push(q);
            }
        }
    }

    return proximity_points;
}

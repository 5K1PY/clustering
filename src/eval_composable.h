#pragma once

#include <queue>
#include <unordered_set>
#include <unordered_map>

#include "hashing.h"
#include "points.h"
#include "r_p.h"
#include "composable.h"

using namespace std;

template<typename T>
vector<T> eval_composable(int dim, vector<tagged_point>& points, double radius, Composable::Composable<T>& f) {
    const ull CELL_SIZE = 2.0 * GridHashing::Gamma(dim) * radius * scale;
    GridHashing hashing_scheme(dim, CELL_SIZE);

    for (tagged_point &p: points) {
        p.hash = hashing_scheme.hash(p);
    }

    unordered_map<ull, T> bucket_f;
    for (tagged_point &p: points) {
        if (bucket_f.find(p.hash) == bucket_f.end())
            bucket_f[p.hash] = f.empty_value;
        bucket_f[p.hash] = f.compose(bucket_f[p.hash], f.evaluate(p));
    }

    vector<T> proximity_points(points.size(), f.empty_value);
    for (int points_i=0; points_i<(int) points.size(); points_i++) {
        queue<point> neighborhood;
        neighborhood.push(points[points_i]);
        unordered_set<ull> found_cells;

        while (neighborhood.size()) {
            point p = neighborhood.front(); neighborhood.pop();
            ull hash = hashing_scheme.hash(p);

            if (found_cells.count(hash) > 0)
                continue;
            found_cells.insert(hash);

            proximity_points[points_i] = f.compose(
                proximity_points[points_i],
                bucket_f[hash]
            );

            for (int ix=0; ix<2*dim; ix++) {
                int i = ix / 2;
                ll direction = 2*(ix % 2) - 1;

                point q = p;
                q[i] += direction*CELL_SIZE;
                if (hashing_scheme.bucket_sphere_intersect(points[points_i], radius, q))
                    neighborhood.push(q);
            }
        }
    }

    return proximity_points;
}

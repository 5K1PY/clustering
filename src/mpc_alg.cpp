#pragma once

#include <queue>
#include <unordered_set>
#include <unordered_map>

#include "hashing.h"
#include "points.h"
#include "r_p.h"

using namespace std;

const int CELL_SIZE = 1e5;

vector<int> mpc_alg(int dim, vector<tagged_point>& points, ull r) {
    GridHashing hashing_scheme(dim, CELL_SIZE);

    for (auto p: points) {
        p.hash = hashing_scheme.hash(p);
    }

    unordered_map<ull, ull> bucket_size;
    for (auto p: points) {
        if (bucket_size.find(p.hash) == bucket_size.end())
            bucket_size[p.hash] = 0;
        bucket_size[p.hash]++;
    }

    vector<int> proximity_points(points.size(), 0);
    for (int points_i=0; points_i<points.size(); points_i++) {
        queue<point> neighborhood;
        // TODO: Start from all points where we round all coordinates to nearest cell hyperplane
        // Because we can miss some cells without it
        neighborhood.push(points[points_i]);
        unordered_set<ull> found_cells;

        while (neighborhood.size()) {
            point p = neighborhood.back(); neighborhood.pop();
            ull hash = hashing_scheme.hash(p);

            if (found_cells.count(hash))
                continue;

            proximity_points[points_i] += bucket_size[hash];

            for (int i=0; i<dim; i++) {
                point q = p;
                q[i] += CELL_SIZE;
                if (points[i].dist_squared(q) <= r*r)
                    neighborhood.push(q);
            }
        }
    }

    return proximity_points;
}

#pragma once

#include <map>

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

    map<ull, ull> bucket_size;
    for (auto p: points) {
        if (bucket_size.find(p.hash) == bucket_size.end())
            bucket_size[p.hash] = 0;
        bucket_size[p.hash]++;
    }

    // TODO: Fix this as this should use euclid distance and not maximal
    vector<int> proximity_points;
    for (auto p: points) {
        proximity_points.push_back(bucket_size[p.hash]);

        point offset(dim);
        for (int i; i<dim; i++) {
            offset.coords[i] = -r;
        }

        while (true) {
            ull hash = hashing_scheme.hash(p + offset);
            if (bucket_size.find(hash) != bucket_size.end()) {
                proximity_points.back() += bucket_size[hash];
            }

            int i = 0;
            for (; i<dim; i++) {
                if (offset.coords[i] > r) {
                    offset.coords[i] = -r;
                } else {
                    offset.coords[i] += CELL_SIZE;
                    break;
                }
            }

            if (i == dim) break;
        }
    }
    return proximity_points;
}

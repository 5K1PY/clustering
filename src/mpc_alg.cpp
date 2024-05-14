#pragma once

#include <map>

typedef unsigned long long ull;
using namespace std;

#include "hashing.h"
#include "points.h"
#include "r_p.h"

const int CELL_SIZE = 1e5;

void mpc_alg(int dim, vector<tagged_point>& points, int r) {
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
    for (auto p: points) {
        int proximity_points = bucket_size[p.hash];

        point offset(dim);
        for (int i; i<dim; i++) {
            offset.coords[i] = -r;
        }

        while (true) {
            ull hash = hashing_scheme.hash(p + offset);
            if (bucket_size.find(hash) != bucket_size.end()) {
                proximity_points += bucket_size[hash];
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
}

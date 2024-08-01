#pragma once

#include <vector>
#include <limits>
#include <random>

#include "points.h"
#include "random.h"

using namespace std;

class GridHashing {
  private:
    int dimension;

    ull cell_size;
    vector<ull> offsets;
    ull hash_poly, hash_mod;

    ull inline normalize_coord(const point& p, int i) {
        return (ull) p.coords[i] - numeric_limits<ll>::min() + offsets[i];
    }
  public:
    static const double Gamma;
    GridHashing(int dim, ull cs) {
        cell_size = cs;
        dimension = dim;

        offsets.resize(dimension, 0);
        for (int i=0; i<dimension; i++) {
            offsets[i] = randRange((ull) 0, numeric_limits<ull>::max());
        }

        hash_poly = numeric_limits<ull>::max() / cell_size + 1;
        hash_mod = ull(1e9)+7;
    }
    ull hash(const point& p) {
        vector<ull> cell(dimension);
        for (int i=0; i<dimension; i++) {
            cell[i] = normalize_coord(p, i) / cell_size;
        }
        ull hash = 0;
        for (int i=0; i<dimension; i++) {
            hash *= hash_poly;
            hash += cell[i];
            hash %= hash_mod;
        }
        return hash;
    }
    bool bucket_sphere_intersect(const point& center, ull radius, point bucket) {
        for (int i=0; i<dimension; i++) {
            ull offset = normalize_coord(bucket, i) % cell_size;
            if (bucket.coords[i] > center.coords[i]) {
                bucket.coords[i] -= offset;
            } else if (bucket.coords[i] < center.coords[i]) {
                bucket.coords[i] += cell_size - offset - 1;
            }
        }
        return center.dist_squared(bucket) <= radius*radius;
    }
};

const double GridHashing::Gamma = sqrt(2.0);

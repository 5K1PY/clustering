#pragma once

#include <vector>
#include <limits>
#include <random>

#include "points.h"
#include "random.h"

using namespace std;

class GridHashing {
  private:
    int _dimension;

    ull _cell_size;
    vector<ull> _offsets;
    ull _hash_poly, _hash_mod = ull(1e9)+7;

    ull inline normalize_coord(const point& p, int i) const {
        return (ull) p.coords[i] - numeric_limits<ll>::min() + _offsets[i];
    }
  public:
    static const double Gamma;

    int const dimension() const { return _dimension; }
    ull const cell_size() const { return _cell_size; }
    ull const hash_poly() const { return _hash_poly; }
    ull const hash_mod() const { return _hash_mod; }

    GridHashing(int dim, ull cs) {
        _cell_size = cs;
        _dimension = dim;

        _offsets.resize(_dimension, 0);
        for (int i=0; i<_dimension; i++) {
            _offsets[i] = randRange((ull) 0, numeric_limits<ull>::max());
        }

        _hash_poly = numeric_limits<ull>::max() / _cell_size + 1;
    }

    GridHashing(int dim, ull cs, const vector<ull> &offsets) {
        _cell_size = cs;
        _dimension = dim;
        _offsets = offsets;
        _hash_poly = numeric_limits<ull>::max() / _cell_size + 1;
    }

    ull hash(const point& p) const {
        vector<ull> cell(_dimension);
        for (int i=0; i<_dimension; i++) {
            cell[i] = normalize_coord(p, i) / _cell_size;
        }
        ull hash = 0;
        for (int i=0; i<_dimension; i++) {
            hash *= _hash_poly;
            hash += cell[i];
            hash %= _hash_mod;
        }
        return hash;
    }

    bool bucket_sphere_intersect(const point& center, double radius, point bucket) const {
        for (int i=0; i<_dimension; i++) {
            ull offset = normalize_coord(bucket, i) % _cell_size;
            if (bucket.coords[i] > center.coords[i]) {
                bucket.coords[i] -= offset;
            } else if (bucket.coords[i] < center.coords[i]) {
                bucket.coords[i] += _cell_size - offset - 1;
            }
        }
        return center.dist_squared(bucket) <= radius * radius;
    }
};

const double GridHashing::Gamma = sqrt(2.0);

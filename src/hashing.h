#pragma once

#include <vector>
#include <limits>
#include <random>

#include "points.h"
#include "random.h"
#include "composable.h"

using namespace std;

template<typename T>
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
    static double const Gamma(int dimension) { return sqrt(dimension); }

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

    T eval_ball(
        const tagged_point& center,
        double radius,
        const Composable::Composable<T>& f,
        const unordered_map<ull, T>& bucket_values
    ) const {
        T result = f.empty_value;

        queue<point> neighborhood;
        neighborhood.push(center);
        unordered_set<ull> found_cells;

        while (neighborhood.size()) {
            point p = neighborhood.front(); neighborhood.pop();
            ull hash_of_p = hash(p);

            if (found_cells.count(hash_of_p) > 0)
                continue;
            found_cells.insert(hash_of_p);

            auto bucket_val = bucket_values.find(hash_of_p);
            if (bucket_val != bucket_values.end()) {
                result = f.compose(result, bucket_val->second);
            }

            for (int ix=0; ix<2*_dimension; ix++) {
                int i = ix / 2;
                ll direction = 2*(ix % 2) - 1;

                point q = p;
                q[i] += direction*_cell_size;
                if (bucket_sphere_intersect(center, radius, q))
                    neighborhood.push(q);
            }
        }
        return result;
    }
};


#pragma once

#include <vector>
#include <limits>
#include <random>
#include <memory>

#include "points.h"
#include "random.h"
#include "composable.h"

using namespace std;

template<typename T>
class Hashing {
  protected:
    ull inline normalize_coord(const point& p, int i) const {
        return (ull) p.coords[i] - numeric_limits<ll>::min();
    }
  public:
    virtual ~Hashing() = default;

    virtual ull hash(const point& p) const = 0;
    virtual T eval_ball(
        const tagged_point& center,
        const double radius,
        const Composable::Composable<T>& f,
        const unordered_map<ull, T>& bucket_values
    ) const = 0;
};

template<typename T>
class GridHashing : public Hashing<T> {
  private:
    int _dimension;

    ull _cell_size;
    vector<ull> _offsets;
    ull _hash_poly, _hash_mod = ull(1e9)+7;
  protected:
    ull inline normalize_coord(const point& p, int i) const {
        return Hashing<T>::normalize_coord(p, i) + _offsets[i];
    }
  public:
    static double Gamma(int dimension) { return sqrt(dimension); }

    int const dimension() const { return _dimension; }
    ull const cell_size() const { return _cell_size; }
    ull const hash_poly() const { return _hash_poly; }
    ull const hash_mod() const { return _hash_mod; }

    GridHashing(int dim, ull radius) {
        _dimension = dim;
        _cell_size = 2.0 * Gamma(dim) * radius;

        _offsets.resize(_dimension, 0);
        for (int i=0; i<_dimension; i++) {
            _offsets[i] = randRange((ull) 0, numeric_limits<ull>::max());
        } 

        _hash_poly = numeric_limits<ull>::max() / _cell_size + 1;
    }

    static GridHashing<T> manual(int dim, ull cs, const vector<ull> &offsets = vector<ull>()) {
        GridHashing<T> gh(dim, 1);
        gh._cell_size = cs;
        if (offsets.size() != 0) {
            gh._offsets = offsets;
        }
        return gh;
    }

    ull hash(const point& p) const override {
        vector<ull> cell(_dimension);
        for (int i=0; i<_dimension; i++) {
            cell[i] = this->normalize_coord(p, i) / _cell_size;
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
        const double radius,
        const Composable::Composable<T>& f,
        const unordered_map<ull, T>& bucket_values
    ) const override {
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

template<typename T>
class FaceHashing : public Hashing<T> {
  private:
    int _dimension;

    ull _hypercube_side;
    ull _epsilon;
    ull _hash_poly, _hash_mod = ull(1e9)+7;
  public:
    static double GammaMul(int dimension) { return 4.0; } // > 1 / dimension + 2
    static double Gamma(int dimension) { return dimension * sqrt(dimension); }

    int const dimension() const { return _dimension; }
    ull const hypercube_side() const { return _hypercube_side; }
    ull const hash_poly() const { return _hash_poly; }
    ull const hash_mod() const { return _hash_mod; }

    FaceHashing(int dim, ull radius) {
        _dimension = dim;
        _hypercube_side = 2.0 * ull(Gamma(dim) / sqrt(dim) * radius);
        _epsilon = 2*radius;

        _hash_poly = numeric_limits<ull>::max() / (_hypercube_side/2) + 1;
    }

    ull hash(const point& p) const override {
        vector<ull> p_norm(_dimension);
        for (int i=0; i<_dimension; i++) {
            p_norm[i] = this->normalize_coord(p, i);
        }

        // distance calculation
        vector<int> epsilon_multiply(_dimension+1, 0);
        for (int i=0; i<_dimension; i++) {
            ull delta = p_norm[i] % _hypercube_side;
            delta = min(delta, _hypercube_side - delta);

            epsilon_multiply[min(int(delta/_epsilon), _dimension)]++;
        }

        // find face dimension
        int mul = -1;
        int points_within = 0;
        for (int x=0; x<_dimension; x++) {
            points_within += epsilon_multiply[x];
            if (points_within >= x)
                mul = x;
        }

        // normalize point
        for (int i=0; i<_dimension; i++) {
            ull alpha = p_norm[i] % _hypercube_side;

            if (alpha <= mul*_epsilon)
                p_norm[i] -= alpha;
            else if (alpha >= _hypercube_side - mul*_epsilon)
                p_norm[i] += _hypercube_side - alpha;
            else
                p_norm[i] += _hypercube_side/2 - alpha;
        }

        // compute hash
        ull hash = 0;
        for (int i=0; i<_dimension; i++) {
            hash *= _hash_poly;
            hash += 2*p_norm[i] / _hypercube_side;
            hash %= _hash_mod;
        }
        return hash;
    }

    T eval_ball(
        const tagged_point& center,
        const double radius,
        const Composable::Composable<T>& f,
        const unordered_map<ull, T>& bucket_values
    ) const override {
        T result = f.empty_value;
        vector<tuple<int, ull, ull>> differences(_dimension);
        for (int i=0; i<_dimension; i++) {
            ull offset = this->normalize_coord(center, i) % _hypercube_side;
            differences[i] = {i, offset, min(offset, _hypercube_side - offset)};
        }
        sort(differences.begin(), differences.end(), [](const auto& p, const auto& q) {
            return get<2>(p) < get<2>(q);
        });

        for (int face_dim=0; face_dim <= _dimension; face_dim++) {
            point closest(center);
            int mul = _dimension - face_dim;
            for (int i=0; i<mul; i++) {
                auto [index, offset, diff] = differences[i];

                if (diff > mul*_epsilon) {
                    if (offset > _hypercube_side / 2) closest[index] += _hypercube_side - offset - mul*_epsilon;
                    else                              closest[index] += mul*_epsilon - offset;
                }
            }
            for (int i=mul; i<_dimension; i++) {
                auto [index, offset, diff] = differences[i];

                if (diff <= mul*_epsilon) {
                    if (offset > _hypercube_side / 2) closest[index] += _hypercube_side - offset - mul*_epsilon - 1;
                    else                              closest[index] += mul*_epsilon - offset + 1;
                }
            }
            if (center.dist(closest) < radius) {
                auto bucket_val = bucket_values.find(hash(closest));
                if (bucket_val != bucket_values.end()) {
                    result = f.compose(result, bucket_val->second);
                }
            }
        }
        return result;
    }
};

enum HashingScheme {GridHashingScheme, FaceHashingScheme};

template<typename T>
unique_ptr<Hashing<T>> make_hashing_scheme(HashingScheme hashing_scheme, int dimension, ull radius) {
    switch (hashing_scheme) {
        case GridHashingScheme: return make_unique<GridHashing<T>>(dimension, radius);
        case FaceHashingScheme: return make_unique<FaceHashing<T>>(dimension, radius);
        default: throw invalid_argument("Unsupported hashing scheme");
    }
}

#pragma once

#include <algorithm>
#include <limits>
#include <memory>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "points.hpp"
#include "random.hpp"
#include "composable.hpp"

template<typename T>
class HashingScheme {
  protected:
    ull inline normalize_coord(const point& p, int i) const {
        return (ull) p.coords[i] - std::numeric_limits<ll>::min();
    }
  public:
    virtual ~HashingScheme() = default;

    virtual ull hash(const point& p) const = 0;
    virtual T eval_ball(
        const tagged_point& center,
        const double radius,
        const Composable::Composable<T>& f,
        const std::unordered_map<ull, T>& bucket_values
    ) const = 0;
};

template<typename T>
class GridHashing : public HashingScheme<T> {
  private:
    int _dimension;

    ull _cell_size;
    std::vector<ull> _offsets;
    ull _hash_poly;
    static constexpr ull _hash_mod = 2147483647;
  protected:
    ull inline normalize_coord(const point& p, int i) const {
        return HashingScheme<T>::normalize_coord(p, i) + _offsets[i];
    }
  public:
    static double Gamma(int dimension) { return sqrt(dimension); }

    const int dimension() const { return _dimension; }
    const ull cell_size() const { return _cell_size; }
    const ull hash_poly() const { return _hash_poly; }
    const ull hash_mod() const { return _hash_mod; }

    GridHashing(int dim, ull radius) {
        _dimension = dim;
        // Setting cell_size to be dim-times bigger actually provides
        // great speedup with better results
        _cell_size = dim * 2.0 * radius;

        _offsets.resize(_dimension, 0);
        for (int i=0; i<_dimension; i++) {
            _offsets[i] = randRange((ull) 0, std::numeric_limits<ull>::max());
        } 

        _hash_poly = randRange(2, std::numeric_limits<int>::max());
    }

    static GridHashing<T> manual(int dim, ull cs, const std::vector<ull> &offsets = std::vector<ull>()) {
        GridHashing<T> gh(dim, 1);
        gh._cell_size = cs;
        if (offsets.size() != 0) {
            gh._offsets = offsets;
        }
        return gh;
    }

    ull hash(const point& p) const override {
        std::vector<ull> cell(_dimension);
        for (int i=0; i<_dimension; i++) {
            cell[i] = this->normalize_coord(p, i) / _cell_size;
        }
        ull hash = 0;
        for (int i=0; i<_dimension; i++) {
            hash *= _hash_poly;
            hash %= _hash_mod;
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
        const std::unordered_map<ull, T>& bucket_values
    ) const override {
        T result = f.empty_value;

        std::queue<point> neighborhood;
        neighborhood.push(center);
        std::unordered_set<ull> found_cells;

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
class FaceHashing : public HashingScheme<T> {
  private:
    int _dimension;

    ull _hypercube_side;
    ull _epsilon;
    ull _hash_poly;
    static constexpr ull _hash_mod = 2147483647;
    static constexpr double gamma_mul = 3.0; // must be >= 3.0 for theoretical guarantees
  public:
    static double Gamma(int dimension) { return gamma_mul * dimension * sqrt(dimension); }

    int const dimension() const { return _dimension; }
    ull const hypercube_side() const { return _hypercube_side; }
    ull const hash_poly() const { return _hash_poly; }
    ull const hash_mod() const { return _hash_mod; }

    FaceHashing(int dim, ull radius) {
        _dimension = dim;
        _hypercube_side = 2*radius * Gamma(dim)/sqrt(dim);
        _epsilon = 2*radius;

        _hash_poly = randRange(2, std::numeric_limits<int>::max());
    }

    ull hash(const point& p) const override {
        std::vector<ull> p_norm(_dimension);
        for (int i=0; i<_dimension; i++) {
            p_norm[i] = this->normalize_coord(p, i);
        }

        // distance calculation
        std::vector<int> epsilon_multiply(_dimension+1, 0);
        for (int i=0; i<_dimension; i++) {
            ull delta = p_norm[i] % _hypercube_side;
            delta = std::min(delta, _hypercube_side - delta);

            epsilon_multiply[std::min(int(delta/_epsilon), _dimension)]++;
        }

        // find face dimension
        int mul = -1;
        int points_within = 0;
        for (int x=1; x<=_dimension; x++) {
            points_within += epsilon_multiply[x-1];
            if (points_within >= x)
                mul = x;
        }

        // normalize point
        for (int i=0; i<_dimension; i++) {
            ull alpha = p_norm[i] % _hypercube_side;

            if (mul != -1 && alpha < mul*_epsilon)
                p_norm[i] -= alpha;
            else if (mul != -1 && alpha > _hypercube_side - mul*_epsilon)
                p_norm[i] += _hypercube_side - alpha;
            else
                p_norm[i] += (_hypercube_side+1)/2 - alpha;
        }

        // compute hash
        ull hash = 0;
        for (int i=0; i<_dimension; i++) {
            hash *= _hash_poly;
            hash %= _hash_mod;
            hash += 2*p_norm[i] / _hypercube_side;
            hash %= _hash_mod;
        }
        return hash;
    }

    T eval_ball(
        const tagged_point& center,
        const double radius,
        const Composable::Composable<T>& f,
        const std::unordered_map<ull, T>& bucket_values
    ) const override {
        T result = f.empty_value;
        std::vector<std::tuple<int, ull, ull>> differences(_dimension);
        for (int i=0; i<_dimension; i++) {
            ull offset = this->normalize_coord(center, i) % _hypercube_side;
            differences[i] = {i, offset, std::min(offset, _hypercube_side - offset)};
        }
        std::sort(differences.begin(), differences.end(), [](const auto& p, const auto& q) {
            return std::get<2>(p) < std::get<2>(q);
        });

        for (int face_dim=0; face_dim <= _dimension; face_dim++) {
            point closest(center);
            int mul = _dimension - face_dim;
            for (int i=0; i<mul; i++) {
                auto [index, offset, diff] = differences[i];

                if (diff >= mul*_epsilon) {
                    if (offset > _hypercube_side / 2) closest[index] += _hypercube_side - offset - mul*_epsilon + 1;
                    else                              closest[index] += mul*_epsilon - offset - 1;
                }
            }
            for (int i=mul; i<_dimension; i++) {
                auto [index, offset, diff] = differences[i];

                if (diff < (i+1)*_epsilon) {
                    if (offset > _hypercube_side / 2) closest[index] += _hypercube_side - offset - (i+1)*_epsilon;
                    else                              closest[index] += (i+1)*_epsilon - offset;
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

enum HashingSchemeChoice {GridHashingScheme, FaceHashingScheme};

double get_gamma(const HashingSchemeChoice hs_choice, int dimension);

template<typename T>
std::unique_ptr<HashingScheme<T>> make_hashing_scheme(HashingSchemeChoice hs_choice, int dimension, ull radius) {
    switch (hs_choice) {
        case GridHashingScheme: return std::make_unique<GridHashing<T>>(dimension, radius);
        case FaceHashingScheme: return std::make_unique<FaceHashing<T>>(dimension, radius);
        default: throw std::invalid_argument("Unsupported hashing scheme");
    }
}

HashingSchemeChoice choose_hashing_scheme(std::string choice);

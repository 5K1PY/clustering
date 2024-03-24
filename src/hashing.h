#include <vector>
#include <limits>
#include <random>

#include "points.h"

using namespace std;

mt19937 rng(76901);
uniform_int_distribution<ull> ull_dist;

class GridHashing {
  private:
    int dimension;

    ull cell_size;
    vector<ull> offsets;
    ull hash_poly, hash_mod;
  public:
    GridHashing(int dim, ull cs) {
        cell_size = cs;
        dimension = dim;

        offsets.resize(dimension, 0);
        for (int i=0; i<dimension; i++) {
            offsets[i] = ull_dist(rng);
        }

        hash_poly = numeric_limits<ull>::max() / cell_size + 1;
        hash_mod = ull(1e9)+7;
    }
    void hash(tagged_point& p) {
        vector<ull> cell(dimension);
        for (int i=0; i<dimension; i++) {
            cell[i] = (p.coords[i] + offsets[i]) / cell_size;
        }
        p.hash = 0;
        for (int i=0; i<dimension; i++) {
            p.hash *= hash_poly;
            p.hash += cell[i];
            p.hash %= hash_mod;
        }
    }
};

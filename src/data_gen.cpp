#include <random>
#include <iostream>

typedef unsigned long long ull;

using namespace std;

mt19937 rng(76901);
uniform_int_distribution<ull> ull_dist;

vector<vector<ull>> gen_random(int n, int dim) {
    vector<vector<ull>> points(n, vector<ull>(dim));
    for (int i=0; i<n; i++) {
        for (int j=0; j<dim; j++) {
            points[i][j] = ull_dist(rng);
        }
    }
    return points;
}

int main() {
    int dimension, n;
    cin >> n >> dimension;

    vector<vector<ull>> points = gen_random(n, dimension);
    for (auto p: points) {
        for (int i=0; i<dimension; i++) {
            cout << p[i] << " ";
        }
        cout << endl;
    }
}

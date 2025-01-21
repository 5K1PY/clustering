#include <iostream>

#include "util.h"
#include "hashing.h"
#include "points.h"
#include "k_median.h"

using namespace std;


int main(int argc, char const *argv[]) {
    if (argc != 3) invalid_usage_solver();
    HashingScheme hashing_scheme = choose_hashing_scheme(argv[1]);
    seed(strtoull(argv[2], 0, 16));

    int n, dim, k;
    cin >> n >> dim >> k;
    auto points = load_points(n, dim);

    auto chosen = compute_clusters_seq(dim, points, k, hashing_scheme);
    for (auto c: chosen) {
        cout << c << " ";
    }
    cout << endl;
}

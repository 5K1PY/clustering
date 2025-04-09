#include <iostream>

#include "lib/util.hpp"
#include "lib/hashing.hpp"
#include "lib/points.hpp"
#include "lib/clustering.hpp"

using namespace std;


int main(int argc, char const *argv[]) {
    if (argc != 3) invalid_usage_solver();
    HashingSchemeChoice hs_choice = choose_hashing_scheme(argv[1]);
    seed(strtoull(argv[2], 0, 16));

    int n, dim, k;
    std::cin >> n >> dim >> k;
    auto points = load_points(n, dim);

    auto chosen = compute_clusters_seq(dim, points, k, hs_choice);
    for (auto c: chosen) {
        std::cout << points[c];
    }
    std::cout << std::endl;
}

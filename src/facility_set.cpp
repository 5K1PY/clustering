#include <iostream>

#include "lib/util.hpp"
#include "lib/hashing.hpp"
#include "lib/points.hpp"
#include "lib/facility_set.hpp"

int main(int argc, char const *argv[]) {
    if (argc != 3) invalid_usage_solver();
    HashingScheme hashing_scheme = choose_hashing_scheme(argv[1]);
    seed(strtoull(argv[2], 0, 16));

    int n, dim; double facility_cost;
    std::cin >> n >> dim >> facility_cost;
    auto points = load_points(n, dim);

    auto chosen = compute_facilities(dim, points, facility_cost, hashing_scheme);
    for (auto c: chosen) {
        std::cout << points[c];
    }
    std::cout << std::endl;
}

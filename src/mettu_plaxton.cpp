#include <iomanip>
#include <iostream>

#include "lib/points.hpp"
#include "lib/random.hpp"
#include "lib/hashing.hpp"
#include "lib/r_p.hpp"

int main(int argc, char const *argv[]) {
    int n, dim; double facility_cost;
    std::cin >> n >> dim >> facility_cost;
    auto points = load_points(n, dim);

    calc_rps(points, facility_cost);

    auto chosen = mettu_plaxton(points);
    std::cout << std::setprecision(15);
    for (auto c: chosen) {
        std::cout << points[c];
    }
}

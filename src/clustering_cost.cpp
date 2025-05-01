#include <fstream>
#include <iomanip>
#include <iostream>

#include "lib/points.hpp"

int main(int argc, char const *argv[]) {
    std::ifstream solution(getenv("SOLUTION"));
    int n, dim, k;
    std::cin >> n >> dim >> k;
    auto points = load_points(n, dim);

    std::vector<point> centers;
    double coord;
    std::vector<double> coords;
    while (solution >> coord) {
        coords.push_back(coord);
        if (coords.size() == (size_t) dim) {
            centers.push_back(point(coords));
            coords.clear();
        }
    }
    double cost = solution_cost(points, centers, 0.0);
    std::cout << std::setprecision(15) << cost << std::endl;
}

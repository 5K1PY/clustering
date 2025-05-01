#include <fstream>
#include <iomanip>
#include <iostream>

#include "lib/points.hpp"

int main(int argc, char const *argv[]) {
    std::ifstream solution(getenv("SOLUTION"));
    int n, dim; double facility_cost;
    std::cin >> n >> dim >> facility_cost;
    auto points = load_points(n, dim);

    std::vector<point> facilities;
    double coord;
    std::vector<double> coords;
    while (solution >> coord) {
        coords.push_back(coord);
        if (coords.size() == (size_t) dim) {
            facilities.push_back(point(coords));
            coords.clear();
        }
    }
    double cost = solution_cost(points, facilities, facility_cost);
    std::cout << std::setprecision(15) << cost << std::endl;
}

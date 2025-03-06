#include <fstream>
#include <limits>

#include "lib/points.hpp"

int main(int argc, char const *argv[]) {
    std::ifstream solution(getenv("SOLUTION"));
    int n, dim; double facility_cost;
    std::cin >> n >> dim >> facility_cost;
    auto points = load_points(n, dim);

    int x = 0;
    std::vector<int> facilities;
    while (solution >> x) {
        facilities.push_back(x);
    }
    double cost = solution_cost(points, facilities, facility_cost);
    std::cout << cost << std::endl;
}

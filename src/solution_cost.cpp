#include <fstream>
#include <limits>

#include "points.h"

using namespace std;

int main(int argc, char const *argv[]) {
    ifstream solution("solution.txt");
    int n, dim; double facility_cost;
    cin >> n >> dim >> facility_cost;
    auto points = load_points(n, dim);

    int x = 0;
    vector<int> facilities;
    while (solution >> x) {
        facilities.push_back(x);
    }

    double cost = facilities.size() * facility_cost;
    for (auto point: points) {
        double min_dist = numeric_limits<double>::infinity();
        for (auto facility_i: facilities) {
            min_dist = min(min_dist, point.dist(points[facility_i]));
        }
        cost += min_dist;
    }

    cout << cost << endl;
}

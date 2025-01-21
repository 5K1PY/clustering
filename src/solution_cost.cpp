#include <fstream>
#include <limits>

#include "points.h"

using namespace std;

int main(int argc, char const *argv[]) {
    ifstream solution(getenv("SOLUTION"));
    int n, dim; double facility_cost;
    cin >> n >> dim >> facility_cost;
    auto points = load_points(n, dim);

    int x = 0;
    vector<int> facilities;
    while (solution >> x) {
        facilities.push_back(x);
    }
    double cost = solution_cost(points, facilities, facility_cost);
    cout << cost << endl;
}

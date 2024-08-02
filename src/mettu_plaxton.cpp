#include "points.h"
#include "random.h"
#include "hashing.h"
#include "r_p.h"

int main(int argc, char const *argv[]) {
    int n, dim; double facility_cost;
    cin >> n >> dim >> facility_cost;
    auto points = load_points(n, dim);

    calc_rps(points, facility_cost);
    for (auto p: points) {
        cout << p.r_p << endl;
    }
    auto chosen = mettu_plaxton(points);
    for (auto c: chosen) {
        cout << c << " ";
    }
    cout << endl;
}

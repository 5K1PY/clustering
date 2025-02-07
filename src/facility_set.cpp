#include <iostream>

#include "util.h"
#include "hashing.h"
#include "points.h"
#include "facility_set.h"

using namespace std;

int main(int argc, char const *argv[]) {
    if (argc != 3) invalid_usage_solver();
    HashingScheme hashing_scheme = choose_hashing_scheme(argv[1]);
    seed(strtoull(argv[2], 0, 16));

    int n, dim; double facility_cost;
    cin >> n >> dim >> facility_cost;
    auto points = load_points(n, dim);

    auto chosen = compute_facilities(dim, points, facility_cost, hashing_scheme);
    for (auto c: chosen) {
        cout << c << " ";
    }
    cout << endl;
}

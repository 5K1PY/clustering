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

    int n, dim, k;
    cin >> n >> dim >> k;
    auto points = load_points(n, dim);

    double facility_cost = binary_search_up<double>([&dim, &points, &hashing_scheme, &k](double guess) {
        return (int) compute_facilities(dim, points, guess, hashing_scheme).size() <= k;
    }, 1e-8, 1e-10);

    auto chosen = compute_facilities(dim, points, facility_cost, hashing_scheme);
    for (auto c: chosen) {
        cout << c << " ";
    }
    cout << endl;
}

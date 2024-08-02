#include "points.h"
#include "facility_set.h"

int main(int argc, char const *argv[]) {
    if (argc >= 2) {
        seed(strtoull(argv[1], 0, 16));
    }

    int n, dim; cin >> n >> dim;
    auto points = load_points(n, dim);
    auto chosen = compute_facilities(dim, points);
    for (auto c: chosen) {
        cout << c << " ";
    }
    cout << endl;
}

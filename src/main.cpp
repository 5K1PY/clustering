#include "points.h"
#include "random.h"
#include "hashing.h"
#include "r_p.h"

int main(int argc, char const *argv[]) {
    int n, dim; cin >> n >> dim;
    auto points = load_points(n, dim);
    calc_rps(points);
    for (auto p: points) {
        cout << p.r_p << endl;
    }
    auto chosen = mettu_plaxton(points);
    for (auto c: chosen) {
        cout << c << " ";
    }
    cout << endl;
}

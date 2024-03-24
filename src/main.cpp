#include "hashing.h"

int main(int argc, char const *argv[]) {
    int n, dim; cin >> n >> dim;
    auto points = load_points(n, dim);
    GridHashing hashing_scheme(dim, (ull) 1e17);
    for (int i=0; i<5; i++) {
        hashing_scheme.hash(points[i]);
        cout << points[i].hash << endl;
    }
    return 0;
}

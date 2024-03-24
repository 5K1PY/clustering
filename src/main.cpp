#include "hashing.h"

int main(int argc, char const *argv[]) {
    auto points = load_points();
    GridHashing hashing_scheme(3, (ull) 1e17);
    for (int i=0; i<5; i++) {
        hashing_scheme.hash(points[i]);
        cout << points[i].hash << endl;
    }
    return 0;
}


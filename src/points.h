#include <vector>
#include <iostream>

using namespace std;

typedef unsigned long long ull;

struct point {
    vector<ull> coords;
    point(int dimension) {
        coords.resize(dimension);
    }

    point operator+(point Y) {
        point result(coords.size());
        for (int i=0; i<coords.size(); i++) {
            result.coords[i] = coords[i] + Y.coords[i];
        }
        return result;
    }

    ull& operator[](size_t idx) {
        return coords[idx];
    }
};

struct tagged_point : point {
    ull hash;

    tagged_point(int dim) : point(dim) {}
};

vector<tagged_point> load_points(int n, int dim) {
    vector<tagged_point> points(n, tagged_point(dim));
    for (int i=0; i<n; i++) {
        for (int j=0; j<dim; j++) {
            cin >> points[i].coords[j];
        }
    }
    return points;
}

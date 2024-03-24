#include <vector>
#include <iostream>

using namespace std;

typedef unsigned long long ull;

struct point {
    vector<ull> coords;
    ull hash;

    point(int dim) {
        coords.resize(dim, 0);
    }    
};

vector<point> load_points() {
    int n, dim;
    cin >> n >> dim;
    vector<point> points(n, point(dim));
    for (int i=0; i<n; i++) {
        for (int j=0; j<dim; j++) {
            cin >> points[i].coords[j];
        }
    }
    return points;
}

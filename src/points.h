#pragma once

#include <math.h>
#include <vector>
#include <iostream>

using namespace std;

typedef unsigned long long ull;
typedef long long ll;

struct point {
    vector<ll> coords;
    point(int dimension) {
        coords.resize(dimension);
    }

    point operator+(const point& Y) {
        point result(coords.size());
        for (int i=0; i<coords.size(); i++) {
            result.coords[i] = coords[i] + Y.coords[i];
        }
        return result;
    }

    ll& operator[](size_t idx) {
        return coords[idx];
    }

    ull dist_squared(const point& Y) {
        ull result = 0;
        for (int i=0; i<coords.size(); i++) {
            ll delta = coords[i] - Y.coords[i];
            result += delta*delta;
        }
        return result;
    }

    double dist(const point& Y) {
        return sqrt(dist_squared(Y));
    }
};

struct tagged_point : point {
    ull hash;
    double r_p;

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

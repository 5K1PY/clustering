#pragma once

#include <math.h>
#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;

typedef unsigned long long ull;
typedef long long ll;

const ll scale = (ll) 1e16;

struct point {
    vector<ll> coords;
    point(int dimension) {
        coords.resize(dimension, 0);
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

    double dist_squared(const point& Y) const {
        double result = 0;
        for (int i=0; i<coords.size(); i++) {
            double delta = (double) coords[i] / scale - (double) Y.coords[i] / scale;
            result += delta*delta;
        }
        return result;
    }

    double dist(const point& Y) const {
        return sqrt(dist_squared(Y));
    }
};

std::ostream& operator<<(std::ostream& os, const point& p) {
    string s = "";
    for (auto c: p.coords) {
        cout << s << double(c) / scale;
        s = " ";
    }
    return cout << "\n";
}

struct tagged_point : point {
    ull hash = 0;
    double r_p = 0.0;
    ull label = 0;

    tagged_point(int dim) : point(dim) {}
};

vector<tagged_point> load_points(int n, int dim) {
    vector<tagged_point> points(n, tagged_point(dim));
    for (int i=0; i<n; i++) {
        for (int j=0; j<dim; j++) {
            double coord;
            cin >> coord;
            points[i].coords[j] = coord * scale;
        }
    }
    return points;
}

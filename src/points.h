#pragma once

#include <algorithm>
#include <assert.h>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <vector>

#include "random.h"

using namespace std;

typedef unsigned long long ull;
typedef long long ll;

const ll scale = (ll) 1e16;

struct point {
    vector<ll> coords;
    point(int dimension) {
        coords.resize(dimension, 0);
    }

    point(vector<double> coordinates) {
        coords.resize(coordinates.size());
        for (int i=0; i<(int) coordinates.size(); i++) {
            coords[i] = coordinates[i] * scale;
        }
    }

    bool operator==(const point& p) const {
        return coords == p.coords;
    }

    bool operator!=(const point& p) const {
        return coords != p.coords;
    }

    point operator+(const point& Y) const {
        point result(coords.size());
        for (int i=0; i<(int) coords.size(); i++) {
            result.coords[i] = coords[i] + Y.coords[i];
        }
        return result;
    }

    ll& operator[](size_t idx) {
        return coords[idx];
    }

    const ll& operator[](size_t idx) const {
        return coords[idx];
    }

    double dist_squared(const point& Y) const {
        double result = 0;
        for (int i=0; i<(int) coords.size(); i++) {
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

struct tagged_point : public point {
    ull hash = 0;
    double r_p = 0.0;
    ull label = 0;

    tagged_point(int dim) : point(dim) {}
};

struct weighted_point : public tagged_point {
    int weight = 0;
    weighted_point(int dim) : tagged_point(dim) {}
    weighted_point(const tagged_point& p) : tagged_point(p) {}
};

struct dist_pair { int index; double dist; };
dist_pair min_dist(const point& p, const vector<tagged_point>& points) {
    int min_i = -1;
    double min_dist2 = std::numeric_limits<double>::infinity();
    for (size_t i=0; i<points.size(); i++) {
        double dist2 = p.dist_squared(points[i]);
        if (dist2 < min_dist2) {
            min_dist2 = dist2;
            min_i = i;
        }
    }
    return {min_i, sqrt(min_dist2)};
}

double solution_cost(const vector<tagged_point>& points, const vector<int>& facilities, double facility_cost) {
    vector<tagged_point> facility_points;
    facility_points.reserve(facilities.size());
    for (auto i: facilities)
        facility_points.push_back(points[i]);

    double cost = facilities.size() * facility_cost;
    for (auto point: points) {
        cost += min_dist(point, facility_points).dist;
    }
    return cost;
}

double nearest_neighbors(int dim, const vector<tagged_point>& points) {
    /* Nearest neighbors using Johnson–Lindenstrauss */
    const int tries = points.size() / 1e2;
    double result = 0;
    for (int _=0; _<tries; _++) {
        vector<double> projection(dim);
        double norm = 0;
        for (int i=0; i<dim; i++) {
            projection[i] = randNormal(0.0, 1.0);
            norm += projection[i]*projection[i];
        }
        norm = sqrt(norm);
        for (int i=0; i<dim; i++) {
            projection[i] /= norm;
        }

        vector<double> projected(points.size(), 0);
        #pragma omp parallel for
        for (size_t i=0; i<points.size(); i++) {
            for (int d=0; d<dim; d++) {
                projected[i] += projection[d] * ((double) points[i][d] / scale);
            }
        }
        sort(projected.begin(), projected.end());
        double min_dist = projected[1] - projected[0];

        for (size_t i=1; i<points.size(); i++) {
            min_dist = min(min_dist, projected[i] - projected[i-1]);
        }
        result = max(result, min_dist);
    }
    return result;
}

double aspect_ratio(int dim, const vector<tagged_point>& points) {
    double min_d = numeric_limits<double>::infinity();  
    double max_d = 0;  
    for (size_t i=0; i<points.size(); i++) {
        for (size_t j=i+1; j<points.size(); j++) {
            double d = points[i].dist(points[j]);
            max_d = max(max_d, d);
            if (d != 0) min_d = min(min_d, d);
        }
    }
    assert(min_d != 0);
    return max_d / min_d;
}

double aspect_ratio_approx(int dim, const vector<tagged_point>& points) {
    /* Approximation of aspect ratio in O(nd + nlogn) */
    point min_coords(dim), max_coords(dim);
    for (int i=0; i<dim; i++) {
        min_coords[i] = std::numeric_limits<ll>::max();
        max_coords[i] = std::numeric_limits<ll>::min();
    }
    for (const point& p: points) {
        for (int i=0; i<dim; i++) {
            min_coords[i] = min(min_coords[i], p.coords[i]);
            max_coords[i] = max(max_coords[i], p.coords[i]);
        }
    }
    return min_coords.dist(max_coords) / nearest_neighbors(dim, points);
}

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

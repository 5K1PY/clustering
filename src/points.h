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

double aspect_ratio(const vector<tagged_point>& points) {
    double min_d = numeric_limits<double>::infinity();  
    double max_d = 0;  
    for (size_t i=0; i<points.size(); i++) {
        for (size_t j=i+1; j<points.size(); j++) {
            max_d = max(max_d, points[i].dist(points[j]));
            min_d = min(min_d, points[i].dist(points[j]));
        }
    }
    // TODO: What if min_d == 0
    return max_d / min_d;
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

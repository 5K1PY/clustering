#pragma once

#include <math.h>
#include <iostream>
#include <vector>

#include "types.hpp"

extern const ll scale;

struct point {
    std::vector<ll> coords;
    point(int dimension) {
        coords.resize(dimension, 0);
    }

    point(std::vector<double> coordinates) {
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

    std::ostream& operator<<(std::ostream& os) {
        std::string s = "";
        for (auto c: coords) {
            std::cout << s << double(c) / scale;
            s = " ";
        }
        return std::cout << "\n";
    }
};

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
dist_pair min_dist(const point& p, const std::vector<tagged_point>& points);
double solution_cost(const std::vector<tagged_point>& points, const std::vector<int>& facilities, double facility_cost);
double nearest_neighbors(int dim, const std::vector<tagged_point>& points);
std::pair<double, double> aspect_ratio(int dim, const std::vector<tagged_point>& points);
std::pair<double, double> aspect_ratio_approx(int dim, const std::vector<tagged_point>& points);
std::vector<tagged_point> load_points(int n, int dim);

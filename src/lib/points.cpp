#include <algorithm>
#include <assert.h>
#include <iostream>
#include <math.h>
#include <vector>

#include "types.hpp"
#include "random.hpp"
#include "points.hpp"

const ll scale = (ll) 1e16;

dist_pair min_dist(const point& p, const std::vector<tagged_point>& points) {
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

double solution_cost(const std::vector<tagged_point>& points, const std::vector<int>& facilities, double facility_cost) {
    std::vector<tagged_point> facility_points;
    facility_points.reserve(facilities.size());
    for (auto i: facilities)
        facility_points.push_back(points[i]);

    double cost = facilities.size() * facility_cost;
    for (auto point: points) {
        cost += min_dist(point, facility_points).dist;
    }
    return cost;
}

double nearest_neighbors(int dim, const std::vector<tagged_point>& points) {
    /* Nearest neighbors using Johnson–Lindenstrauss */
    const int tries = points.size() / 1e2;
    double result = 0;
    for (int _=0; _<tries; _++) {
        std::vector<double> projection(dim);
        double norm = 0;
        for (int i=0; i<dim; i++) {
            projection[i] = randNormal(0.0, 1.0);
            norm += projection[i]*projection[i];
        }
        norm = sqrt(norm);
        for (int i=0; i<dim; i++) {
            projection[i] /= norm;
        }

        std::vector<double> projected(points.size(), 0);
        #pragma omp parallel for
        for (size_t i=0; i<points.size(); i++) {
            for (int d=0; d<dim; d++) {
                projected[i] += projection[d] * ((double) points[i][d] / scale);
            }
        }
        std::sort(projected.begin(), projected.end());
        double min_dist = projected[1] - projected[0];

        for (size_t i=1; i<points.size(); i++) {
            min_dist = std::min(min_dist, projected[i] - projected[i-1]);
        }
        result = std::max(result, min_dist);
    }
    return result;
}

double aspect_ratio(int dim, const std::vector<tagged_point>& points) {
    double min_d = std::numeric_limits<double>::infinity();  
    double max_d = 0;  
    for (size_t i=0; i<points.size(); i++) {
        for (size_t j=i+1; j<points.size(); j++) {
            double d = points[i].dist(points[j]);
            max_d = std::max(max_d, d);
            if (d != 0) min_d = std::min(min_d, d);
        }
    }
    assert(min_d != 0);
    return max_d / min_d;
}

double aspect_ratio_approx(int dim, const std::vector<tagged_point>& points) {
    /* Approximation of aspect ratio in O(nd + nlogn) */
    point min_coords(dim), max_coords(dim);
    for (int i=0; i<dim; i++) {
        min_coords[i] = std::numeric_limits<ll>::max();
        max_coords[i] = std::numeric_limits<ll>::min();
    }
    for (const point& p: points) {
        for (int i=0; i<dim; i++) {
            min_coords[i] = std::min(min_coords[i], p.coords[i]);
            max_coords[i] = std::max(max_coords[i], p.coords[i]);
        }
    }
    return min_coords.dist(max_coords) / nearest_neighbors(dim, points);
}

std::vector<tagged_point> load_points(int n, int dim) {
    std::vector<tagged_point> points(n, tagged_point(dim));
    for (int i=0; i<n; i++) {
        for (int j=0; j<dim; j++) {
            double coord;
            std::cin >> coord;
            points[i].coords[j] = coord * scale;
        }
    }
    return points;
}

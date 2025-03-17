#include <algorithm>
#include <vector>
#include <limits>
#include <assert.h>

#include "points.hpp"
#include "facility_set.hpp"
#include "pow_z.hpp"

typedef unsigned long long ull;

std::vector<weighted_point> group_centers(const std::vector<tagged_point>& points, const std::vector<tagged_point>& approx_k_facilities) {
    std::vector<weighted_point> weighted_points;
    for (auto p: approx_k_facilities) {
        weighted_points.push_back(weighted_point(p));
    }
    for (auto p: points) {
        weighted_points[min_dist(p, approx_k_facilities).index].weight++;
    }
    return weighted_points;
}

std::vector<int> compute_clusters_seq(int dim, std::vector<tagged_point> points, int k, HashingScheme hashing_scheme, double mu=0.4) {
    assert(k >= 1);
    assert(0 <= mu && mu <= 1);

    double opt_guess = -1;
    double min_cost = std::numeric_limits<double>::infinity();
    auto [min_d, max_d] = aspect_ratio_approx(dim, points);
    for (double guess=POWZ(min_d); guess < points.size()*POWZ(max_d); guess*=2) {
        assert(guess > 0);
        double facility_cost = guess / k;
        auto facilities_indexes = compute_facilities(dim, points, facility_cost, hashing_scheme);
        if (facilities_indexes.size() > 2*get_gamma(hashing_scheme, dim)*k) continue;
        double cost = solution_cost(points, facilities_indexes, facility_cost);
        if (min_cost > cost) {
            min_cost = cost;
            opt_guess = guess;
        }
    }
    assert(opt_guess != -1);
    auto facilities_indexes = compute_facilities(dim, points, opt_guess / k, hashing_scheme);

    std::vector<tagged_point> approx_k_facilities;
    approx_k_facilities.reserve(facilities_indexes.size());
    for (int i: facilities_indexes) {
        approx_k_facilities.push_back(points[i]);
    }
    auto wp = group_centers(points, approx_k_facilities);
    std::vector<std::pair<int, weighted_point>> weighted_points;
    weighted_points.reserve(wp.size());
    for (size_t i=0; i<wp.size(); i++) {
        weighted_points.push_back({facilities_indexes[i], wp[i]});
    }

    std::sort(
        weighted_points.begin(),
        weighted_points.end(),
        [](auto& wp1, auto& wp2) { return wp1.second.weight > wp2.second.weight; }
    );

    std::vector<int> result;
    std::vector<tagged_point> centers;
    for (size_t i=0; i<weighted_points.size(); i++) {
        weighted_point p = weighted_points[i].second;
        if (result.size() == 0 || POWZ(min_dist(p, centers).dist) * p.weight > POWZ(2) * opt_guess / (mu*k)) {
            result.push_back(weighted_points[i].first);
            centers.push_back(p);
        }
    }
    return result;
}

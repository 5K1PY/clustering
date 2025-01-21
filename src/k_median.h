#pragma once

#include <limits>
#include <cassert>

#include "random.h"
#include "points.h"
#include "composable.h"
#include "eval_composable.h"
#include "bin_search.h"
#include "facility_set.h"

typedef unsigned long long ull;

vector<weighted_point> group_centers(const vector<tagged_point>& points, const vector<tagged_point>& approx_k_facilities) {
    vector<weighted_point> weighted_points;
    for (auto p: approx_k_facilities) {
        weighted_points.push_back(weighted_point(p));
    }
    for (auto p: points) {
        weighted_points[min_dist(p, approx_k_facilities).index].weight++;
    }
    return weighted_points;
}

vector<int> compute_clusters_seq(int dim, vector<tagged_point> points, int k, HashingScheme hashing_scheme, double mu=0.5) {
    assert(0 <= mu && mu <= 1);

    double opt_guess = -1;
    double min_cost = numeric_limits<double>::infinity();
    double delta = aspect_ratio(points);
    for (int guess=1; guess < points.size()*delta; guess*=2) {
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

    vector<tagged_point> approx_k_facilities;
    approx_k_facilities.reserve(facilities_indexes.size());
    for (int i: facilities_indexes) {
        approx_k_facilities.push_back(points[i]);
    }
    auto weighted_points = group_centers(points, approx_k_facilities);
    
    sort(
        weighted_points.begin(),
        weighted_points.end(),
        [](const weighted_point& p1, const weighted_point& p2) { return p1.weight > p2.weight; }
    );

    vector<int> result;
    vector<tagged_point> centers;
    for (size_t i=0; i<weighted_points.size(); i++) {
        weighted_point p = weighted_points[i];
        if (result.size() == 0 || min_dist(p, centers).dist * p.weight > 2 * opt_guess / (mu*k)) {
            result.push_back(i); // TODO return actual index
            centers.push_back(p);
        }
    }
    for (auto p: centers) cout << p;
    return result;
}

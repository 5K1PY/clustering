#pragma once

#include <utility>
#include <vector>
#include <algorithm>

#include "points.h"
#include "bin_search.h"

using namespace std;

double calc_rp_first_k(vector<tagged_point>& points, tagged_point from, int k, double facility_cost) {
    double rp = facility_cost;
    for (int i=0; i<k; i++) {
        rp += from.dist(points[i]);
    }
    return rp / k;
}

double calc_rp(vector<tagged_point>& points, int from, double facility_cost) {
    vector<tagged_point> copied_points(points);
    sort(
        copied_points.begin(), copied_points.end(),
        [&points, &from](const point& X, const point& Y){
            return points[from].dist_squared(X) < points[from].dist_squared(Y);
        }
    );

    int included = binary_search<int>(
        [&copied_points, &points, &from, &facility_cost](int mid) {
            double rp_mid = calc_rp_first_k(copied_points, points[from], mid+1, facility_cost);
            return rp_mid < points[from].dist(copied_points[mid]);
        },
        0, copied_points.size()
    );

    return calc_rp_first_k(copied_points, points[from], included, facility_cost);
}

void calc_rps(vector<tagged_point>& points, double facility_cost) {
    for (int i=0; i<(int) points.size(); i++) {
        points[i].r_p = calc_rp(points, i, facility_cost);
    }
}

vector<int> mettu_plaxton(vector<tagged_point>& original_points) {
    vector<pair<int, tagged_point*>> points(original_points.size());
    for (int i=0; i<(int) original_points.size(); i++) {
        points[i] = {i, &original_points[i]};
    }

    vector<int> chosen;
    sort(
        points.begin(), points.end(),
        [](const pair<int, tagged_point*>& X, const pair<int, tagged_point*>& Y) {
            return X.second->r_p < Y.second->r_p;
        }
    );

    for (auto [i, p]: points) {
        bool add = true;
        for (int j: chosen) {
            add &= p->dist_squared(original_points[j]) > 4*p->r_p*p->r_p;
            if (!add) break;
        }
        if (add) {
            chosen.push_back(i);
        }
    }

    return chosen;
}

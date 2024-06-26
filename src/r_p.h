#pragma once

#include <utility>
#include <vector>
#include <algorithm>

#include "points.h"

using namespace std;

double calc_rp_first_k(vector<tagged_point>& points, tagged_point from, int k) {
    double rp = 1.0;
    for (int i=0; i<k; i++) {
        rp += from.dist(points[i]);
    }
    return rp / k;
}

double calc_rp(vector<tagged_point>& points, int from) {
    vector<tagged_point> copied_points(points);
    sort(
        copied_points.begin(), copied_points.end(),
        [&points, &from](const point& X, const point& Y){
            return points[from].dist_squared(X) < points[from].dist_squared(Y);
        }
    );

    int l = 0;
    int r = copied_points.size();
    while (l + 1 < r) {
        int mid = (l+r)/2;
        double rp_mid = calc_rp_first_k(copied_points, points[from], mid+1);
        if (rp_mid < points[from].dist(copied_points[mid])) {
            r = mid;
        } else {
            l = mid;
        }
    }
    return calc_rp_first_k(copied_points, points[from], l+1);
}

void calc_rps(vector<tagged_point>& points) {
    for (int i=0; i<points.size(); i++) {
        points[i].r_p = calc_rp(points, i);
    }
}

vector<int> mettu_plaxton(vector<tagged_point>& original_points) {
    vector<pair<int, tagged_point*>> points(original_points.size());
    for (int i=0; i<original_points.size(); i++) {
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

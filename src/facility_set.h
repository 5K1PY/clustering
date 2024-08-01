#pragma once

#include <limits>

#include "random.h"
#include "points.h"
#include "eval_composable.h"
#include "bin_search.h"

typedef unsigned long long ull;

const float tau_param = 1.0;
const float f_param = 1.0;
const float beta_param = 1.0;

vector<int> compute_facilities(int dim, vector<tagged_point> points) {
    for (auto &p: points) {
        p.label = randRange(0ULL, numeric_limits<ull>::max());
    }
    
    vector<int> r_approx(points.size(), 0);
    vector<const tagged_point*> min_labels(points.size(), NULL);
    
    ull r_guess = 1;
    while (find(r_approx.begin(), r_approx.end(), 0) != r_approx.end()) {
        vector<int> approx_ball_sizes = eval_composable(dim, points, r_guess, Composable::Size);
        vector<const tagged_point*> guess_min_labels = eval_composable(dim, points, r_guess, Composable::MinLabel);
        for (int i=0; i<points.size(); i++) {
            if (r_approx[i] == 0 && approx_ball_sizes[i] >= f_param * beta_param * r_guess) {
                r_approx[i] = approx_ball_sizes[i];
                min_labels[i] = guess_min_labels[i];
            }
        }
        r_guess *= 2;
    }

    vector<int> results;
    for (int i=0; i<points.size(); i++) {
        if (&points[i] == min_labels[i] || randBool(tau_param * r_approx[i] / f_param))
            results.push_back(i);
    }
    return results;
}

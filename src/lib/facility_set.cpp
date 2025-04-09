#include <limits>

#include "types.hpp"
#include "random.hpp"
#include "points.hpp"
#include "composable.hpp"
#include "eval_composable.hpp"
#include "facility_set.hpp"
#include "pow_z.hpp"

const double beta_mul = 0.25;

std::vector<int> compute_facilities(int dim, std::vector<tagged_point> points, double facility_cost, HashingSchemeChoice hs_choice) {
    for (auto &p: points) {
        p.label = randRange(0ULL, std::numeric_limits<ull>::max());
    }
    
    std::vector<double> r_approx(points.size(), 0);
    std::vector<const tagged_point*> min_labels(points.size(), NULL);

    double r_guess = 1.0 / scale;
    double beta = beta_mul * 3.0 * get_gamma(hs_choice, dim);
    double tau = pow(beta, 0.25*Z);
    while (find(r_approx.begin(), r_approx.end(), 0) != r_approx.end()) {
        std::vector<int> approx_ball_sizes = eval_composable(dim, points, r_guess, Composable::Size, hs_choice);
        std::vector<const tagged_point*> guess_min_labels = eval_composable(dim, points, r_guess, Composable::MinLabel, hs_choice);
        for (int i=0; i<(int) points.size(); i++) {
            if (r_approx[i] != 0) continue;
            if (approx_ball_sizes[i] >= facility_cost / (2 * POWZ(beta) * POWZ(r_guess))) {
                r_approx[i] = r_guess;
                min_labels[i] = guess_min_labels[i];
            } else if (approx_ball_sizes[i] == (int) points.size()) {
                r_approx[i] = INVPOWZ(facility_cost / (2 * POWZ(beta) * points.size()));
                min_labels[i] = guess_min_labels[i];
            }
        }
        r_guess *= 2;
    }

    std::vector<int> results;
    for (int i=0; i<(int) points.size(); i++) {
        if (&points[i] == min_labels[i] || randBool(POWZ(tau) * POWZ(r_approx[i]) / facility_cost))
            results.push_back(i);
    }
    return results;
}

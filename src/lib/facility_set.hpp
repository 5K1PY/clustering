#include "hashing.hpp"

/**
 * @brief Computes set of facilities to open for some set of points P.
 *
 * See https://arxiv.org/pdf/2307.07848 Algorithm 2.
 *
 * @param dim The dimension of the space.
 * @param points The set of points P.
 * @param facility_cost The cost per one opened facility.
 * @param hs_choice The choice of hashing scheme to use.
 * @return Set of facilities as indexes into set of points P.
 */
std::vector<int> compute_facilities(int dim, std::vector<tagged_point> points, double facility_cost, HashingSchemeChoice hs_choice);

#include "hashing.hpp"

const double tau_param = 1.0;
// TODO: Beta has to satisfy:
// B_P(p, r) ⊆ A_P^β(p, r) ⊆ BP(p, βr).
const double beta_param = 1.0;

std::vector<int> compute_facilities(int dim, std::vector<tagged_point> points, double facility_cost, HashingScheme hashing_scheme);

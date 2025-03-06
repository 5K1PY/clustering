#include "hashing.hpp"

const float tau_param = 1.0;
const float beta_param = 1.0;

std::vector<int> compute_facilities(int dim, std::vector<tagged_point> points, double facility_cost, HashingScheme hashing_scheme);

#include <vector>

#include "points.hpp"
#include "hashing.hpp"

std::vector<weighted_point> group_centers(const std::vector<tagged_point>& points, const std::vector<tagged_point>& approx_k_facilities);
std::vector<int> compute_clusters_seq(int dim, std::vector<tagged_point> points, int k, HashingScheme hashing_scheme, double mu=0.4);

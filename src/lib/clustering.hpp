#include <vector>

#include "points.hpp"
#include "hashing.hpp"

/**
 * @brief Moves every point to a the nearest facility to construct a coreset of weighted points.
 *
 * See https://arxiv.org/pdf/2307.07848 Section 5.1
 *
 * @param points The set of points.
 * @param approx_k_facilities Facilities to move 
 * @return The coreset of weighted points.
 */
std::vector<weighted_point> group_centers(const std::vector<tagged_point>& points, const std::vector<tagged_point>& approx_k_facilities);

/**
 * @brief Sequential algorithm for weak coresets.
 *
 * See https://arxiv.org/pdf/2307.07848 Algorithm 6
 *
 * @param weighted_points The coreset of weighted points with their original indexes.
 * @param k How many clusters to create.
 * @param mu The approximation parameter for the number of clusters.
 * @param guess A guess that 2-approximates optimal solution cost for weak coresets.
 * @return Set of cluster centers as original indexes.
 */
std::vector<int> weak_coresets_seq(const std::vector<std::pair<int, weighted_point>>& weighted_points, const int k, const double mu, const double guess);

/**
 * @brief Sequential algorithm for clustering.
 *        Uses Reduction to weak coresets (5.1) and the Sequential algorithm for weak coresets (5.2)
 *
 *        Note that this algorithm can return up to (1+𝜇)k clusters.
 *
 * See https://arxiv.org/pdf/2307.07848 Section 5
 *
 * @param dim The dimension of the space.
 * @param points The set of points P.
 * @param k How many clusters to create.
 * @param hs_choice The choice of hashing scheme to use.
 * @param mu The approximation parameter for the number of clusters.
 *           The algorithm returns up to (1+𝜇)k and the cost of the solution scales with respect to 1/𝜇.
 * @return Set of cluster centers as indexes into the set of points P.
 */
std::vector<int> compute_clusters_seq(int dim, std::vector<tagged_point> points, int k, HashingSchemeChoice hs_choice, double mu=0.1);

#include <vector>

#include "points.hpp"

/**
 * @brief Calculates r_p for a single point if only k closest points existed.
 * @param points The set of points.
 * @param from The index of the point into `points` for which to calculate r_p.
 * @param k How many closest points to consider
 * @param facility_cost The cost per one opened facility.
 * @return r_p of the given point if only k closest points existed
 */
double calc_rp_first_k(std::vector<tagged_point>& points, tagged_point from, int k, double facility_cost);

/**
 * @brief Calculates r_p for a single point in O(nlogn).
 * @param points The set of points.
 * @param from The index of the point into `points` for which to calculate r_p.
 * @param facility_cost The cost per one opened facility.
 * @return r_p of the given point
 */
double calc_rp(std::vector<tagged_point>& points, int from, double facility_cost);

/**
 * @brief Calculates r_p for all points in O(n^2logn).
 * @param points The set of points. (This vector is modified -- r_p is set in every element!)
 * @param facility_cost The cost per one opened facility.
 */
void calc_rps(std::vector<tagged_point>& points, double facility_cost);

/**
 * @brief Mettu-Plaxton algorithm for facility location.
 *        Gives O(1)-approximate solution.
 *
 * @param original_points The set of points. Points must have their r_ps already set.
 * @return Set of facilities as indexes into the set of points.
 */
std::vector<int> mettu_plaxton(std::vector<tagged_point>& original_points);

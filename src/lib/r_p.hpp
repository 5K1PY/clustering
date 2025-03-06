#include <vector>

#include "points.hpp"

double calc_rp_first_k(std::vector<tagged_point>& points, tagged_point from, int k, double facility_cost);
double calc_rp(std::vector<tagged_point>& points, int from, double facility_cost);
void calc_rps(std::vector<tagged_point>& points, double facility_cost);
std::vector<int> mettu_plaxton(std::vector<tagged_point>& original_points);

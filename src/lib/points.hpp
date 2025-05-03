#pragma once

#include <concepts>
#include <math.h>
#include <iostream>
#include <vector>

#include "types.hpp"

/// Global scaling factor for coordinates
extern const ll scale;

/**
 * @brief Represents a point in a multidimensional space.
 */
struct point {
    std::vector<ll> coords;

    point(int dimension) {
        coords.resize(dimension, 0);
    }

    point(std::vector<double> coordinates) {
        coords.resize(coordinates.size());
        for (int i=0; i<(int) coordinates.size(); i++) {
            coords[i] = coordinates[i] * scale;
        }
    }

    bool operator==(const point& p) const {
        return coords == p.coords;
    }

    bool operator!=(const point& p) const {
        return coords != p.coords;
    }

    point operator+(const point& p) const {
        point result(coords.size());
        for (int i=0; i<(int) coords.size(); i++) {
            result.coords[i] = coords[i] + p.coords[i];
        }
        return result;
    }

    ll& operator[](size_t idx) {
        return coords[idx];
    }

    const ll& operator[](size_t idx) const {
        return coords[idx];
    }

    double dist_squared(const point& p) const {
        double result = 0;
        for (int i=0; i<(int) coords.size(); i++) {
            double delta = (double) coords[i] / scale - (double) p.coords[i] / scale;
            result += delta*delta;
        }
        return result;
    }

    double dist(const point& p) const {
        return sqrt(dist_squared(p));
    }

    friend std::ostream& operator<<(std::ostream& os, const point& p) {
        std::string s = "";
        for (auto c: p.coords) {
            std::cout << s << double(c) / scale;
            s = " ";
        }
        return std::cout << "\n";
    }
};

/**
 * @brief Represents a point with additional metadata.
 */
struct tagged_point : public point {
    ull hash = 0; ///< Hash value for the point
    double r_p = 0.0; ///< r_p value for the point
    ull label = 0; ///< Label of the point (used in step P2 when selecting point with the smallest label)

    tagged_point(int dim) : point(dim) {}
};

/**
 * @brief Represents a weighted point that was created by replacing multiple points
 */
struct weighted_point : public tagged_point {
    int weight = 0; ///< How many points were replaced by this point

    weighted_point(int dim) : tagged_point(dim) {}
    weighted_point(const tagged_point& p) : tagged_point(p) {}
};

template <typename T>
concept IsPoint = std::is_base_of_v<point, T>;

/**
 * @brief Represents point and distance to it from some other point.
 */
struct dist_pair {
    int index; ///< Index of the point
    double dist; ///< Distance to the point
};

/**
 * @brief Finds the point with the minimum distance to a given point.
 * @tparam T The type of points in the vector.
 * @param p The given point.
 * @param points The vector of points to search.
 * @return A pair containing the index and distance of the closest point.
 */
template <IsPoint T>
dist_pair min_dist(const point& p, const std::vector<T>& points) {
    int min_i = -1;
    double min_dist2 = std::numeric_limits<double>::infinity();
    for (size_t i=0; i<points.size(); i++) {
        double dist2 = p.dist_squared(points[i]);
        if (dist2 < min_dist2) {
            min_dist2 = dist2;
            min_i = i;
        }
    }
    return {min_i, sqrt(min_dist2)};
}

/**
 * @brief Computes the cost of a solution given points and facilities.
 * @param points The set of points.
 * @param facilities The built facilities.
 * @param facility_cost Cost per one facility.
 * @return The total cost of the solution.
 */
double solution_cost(const std::vector<tagged_point>& points, const std::vector<point>& facilities, double facility_cost);

/**
 * @brief Computes the cost of a solution given points and facilities which are built on top the points.
 * @param points The set of points.
 * @param facility_indexes Indexes of points on which to build facilities.
 * @param facility_cost Cost per one facility.
 * @return The total cost of the solution.
 */
double solution_cost(const std::vector<tagged_point>& points, const std::vector<int>& facility_indexes, double facility_cost);

/**
 * @brief Approximates distance between two closest points using Johnson–Lindenstrauss.
 * @param dim The dimension of the space.
 * @param points The set of points.
 * @return The nearest neighbor distance.
 */
double nearest_neighbors(int dim, const std::vector<tagged_point>& points);

/**
 * @brief Computes the minimum and maximum distance of a set of points in O(n^2d).
 * @param dim The dimension of the space.
 * @param points The set of points.
 * @return A pair containing the minimum and maximum distances.
 */
std::pair<double, double> aspect_ratio(int dim, const std::vector<tagged_point>& points);

/**
 * @brief Approximates the minimum and maximum of a set of points in time O(nd + nlogn).
 *
 * - Maximum distance is approximated using maximum distance in each coordinate.
 * - Minimum distance is approximated using Johnson–Lindenstrauss.
 *
 * @param dim The dimension of the space.
 * @param points The set of points.
 * @return A pair containing the approximate minimum and maximum distances.
 */
std::pair<double, double> aspect_ratio_approx(int dim, const std::vector<tagged_point>& points);

/**
 * @brief Loads a set of points from std::cin.
 * @param n The number of points to load.
 * @param dim The dimension of the space.
 * @return A vector of loaded points.
 */
std::vector<tagged_point> load_points(int n, int dim);
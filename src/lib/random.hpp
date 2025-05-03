#pragma once

#include <random>

#include "types.hpp"

extern std::mt19937 rng;

/**
 * @brief Initialize the random number generator.
 * @param seed The initialization seed
 */
void seed(ull seed);

/**
 * @brief Generates random integer from [min, max] inclusive
 * @param min The lower bound (inclusive)
 * @param max The upper bound (inclusive)
 * @return A random integer between [min, max] inclusive
 */
template<typename T> T randRange(T min, T max) {
    std::uniform_int_distribution<T> dist(min, max);
    return dist(rng);
}

/**
 * @brief Generates random value from normal distribution N(mean, stddev^2)
 * @param mean The mean of the normal distribution
 * @param stddev The standard deviation of the normal distribution
 * @return A random value from N(mean, stddev^2)
 */
template<typename T> T randNormal(T mean, T stddev) {
    std::normal_distribution<double> dist((double) mean, (double) stddev);
    return (T) dist(rng);
}

/**
 * @brief Generates random double from [from, to).
 * @param from The lower bound (inclusive)
 * @param to The upper bound (exclusive)
 * @return A random double from [from, to)
 */
double randDouble(double from, double to);

/**
 * @brief Generates true with probability p.
 * @param p The probability of true
 * @return true with probability p, false otherwise
 */
bool randBool(double p);

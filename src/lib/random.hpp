#pragma once

#include <random>

#include "types.hpp"

extern std::mt19937 rng;

void seed(ull seed);
template<typename T> T randRange(T min, T max) {
    /**
     * @brief Generates random integer from [min, max] inclusive
     * 
     * @param min lower bound (inclusive)
     * @param max upper bound (inclusive)
     * @return random integer between [min, max] inclusive
     */
    std::uniform_int_distribution<T> dist(min, max);
    return dist(rng);
}

template<typename T> T randNormal(T mean, T stddev) {
    /**
     * @brief Generates random value from normal distribution N(mean, stddev)
     * 
     * @param mean   mean of the normal distribution
     * @param stddev standard deviation of the normal distribution
     * @return random value from N(mean, stddev)
     */
    std::normal_distribution<double> dist((double) mean, (double) stddev);
    return (T) dist(rng);
}

double randDouble(double from, double to);
bool randBool(double p);

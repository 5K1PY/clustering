#pragma once

#include <random>
#include <limits>
#include <type_traits>

using namespace std;

mt19937 rng(76901);

template<typename T> T randRange(T min, T max) {
    /**
     * @brief Generates random integer from [min, max] inclusive
     * 
     * @param min lower bound (inclusive)
     * @param max upper bound (inclusive)
     * @return random integer between [min, max] inclusive
     */
    uniform_int_distribution<T> dist(min, max);
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
    normal_distribution<double> dist((double) mean, (double) stddev);
    return (T) dist(rng);
}

double randDouble(double from, double to) {
    /**
     * @brief Generates random double from [from, to)
     * 
     * @param from lower bound (inclusive)
     * @param to   upper bound (exclusive)
     * @return random double from [from, to)
     */
    std::uniform_real_distribution<double> dist(from, to);
    return dist(rng);
}

bool randBool(double p = 0.5) {
    /**
     * @brief Generates true with probability p
     * 
     * @param p probability of true
     * @return true with probability p, false otherwise 
     */
    return randDouble(0, 1) <= p;
}


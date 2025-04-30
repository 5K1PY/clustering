#include <random>

#include "types.hpp"
#include "random.hpp"

std::mt19937 rng(76901);

void seed(ull seed) {
    rng = std::mt19937(seed);
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


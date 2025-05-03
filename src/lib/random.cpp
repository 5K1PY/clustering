#include <random>

#include "types.hpp"
#include "random.hpp"

std::mt19937 rng(76901);

void seed(ull seed) {
    rng = std::mt19937(seed);
}

double randDouble(double from, double to) {
    std::uniform_real_distribution<double> dist(from, to);
    return dist(rng);
}

bool randBool(double p = 0.5) {
    return randDouble(0, 1) <= p;
}


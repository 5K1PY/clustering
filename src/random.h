#pragma once

#include <random>
#include <limits>
#include <type_traits>

using namespace std;

mt19937 rng(76901);

template<typename T> T randRange(T min, T max) {
    uniform_int_distribution<T> dist(min, max);
    return dist(rng);
}

template<typename T> T randNormal(T mean, T stddev) {
    normal_distribution<double> dist((double) mean, (double) stddev);
    return (T) dist(rng);
}

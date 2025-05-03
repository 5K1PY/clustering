#include <functional>

/**
 * @brief Searches for first number matching a predicate.
 * @tparam T The type of numbers to search.
 * @param predicate The predicate. Must be non-decreasing.
 * @param lower The lowest possible number. For lower-epsilon the predicate mustn't hold.
 * @param upper A number for which the predicate holds.
 * @param epsilon The search density.
 * @return Number x, such that predicate holds for x and doesn't hold x-epsilon.
 */
template<typename T>
T binary_search(const std::function<bool(T)>& predicate, T lower, T upper, T epsilon=1) {
    lower -= epsilon;
    while (lower + epsilon < upper) {
        T mid = (lower + upper) / 2;
        if (predicate(mid)) {
            upper = mid;
        } else {
            lower = mid;
        }
    }
    return lower + epsilon;
}


/**
 * @brief Searches for first number matching a predicate without upper limit.
 * @tparam T The type of numbers to search.
 * @param predicate The predicate. Must be non-decreasing.
 * @param lower The lowest possible number. For lower-epsilon the predicate mustn't hold.
 * @param upper A number for which the predicate holds.
 * @param epsilon The search density.
 */
template<typename T>
T binary_search_up(const std::function<bool(T)>& predicate, T lower, T epsilon=1) {
    T diff = 1;
    while (!predicate(lower + diff)) {
        diff *= 2;
    }
    return binary_search<T>(predicate, lower + diff/2, lower + diff, epsilon);
}

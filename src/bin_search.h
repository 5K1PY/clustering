#pragma once

#include <functional>

using namespace std;
    
template<typename T>
T binary_search(const function<bool(T)>& predicate, T lower, T upper, T epsilon=1) {
    lower--;
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

template<typename T>
T binary_search_up(const function<bool(T)>& predicate, T lower, T epsilon=1) {
    T diff = 1;
    while (!predicate(lower + diff)) {
        diff *= 2;
    }
    return binary_search<T>(predicate, lower + diff/2, lower + diff, epsilon);
}

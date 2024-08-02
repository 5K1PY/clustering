#pragma once

#include <functional>

using namespace std;
    
template<typename T>
T binary_search(const function<bool(T)>& predicate, T lower, T upper) {
    while (lower + 1 < upper) {
        T mid = (lower + upper) / 2;
        if (predicate(mid)) {
            upper = mid;
        } else {
            lower = mid;
        }
    }
    return lower + 1;
}

template<typename T>
T binary_search(const function<bool(T)>& predicate, T lower) {
    T diff = 1;
    while (!predicate(lower + diff)) {
        diff *= 2;
    }
    return binary_search(predicate, lower + diff/2, lower + diff);
}

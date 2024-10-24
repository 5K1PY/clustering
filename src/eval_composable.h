#pragma once

#include <queue>
#include <unordered_set>
#include <unordered_map>

#include "hashing.h"
#include "points.h"
#include "r_p.h"
#include "composable.h"

using namespace std;

template<typename T>
vector<T> eval_composable(int dim, vector<tagged_point>& points, double radius, const Composable::Composable<T>& f) {
    const ull CELL_SIZE = 2.0 * GridHashing<T>::Gamma(dim) * radius * scale;
    GridHashing<T> hashing_scheme(dim, CELL_SIZE);

    for (tagged_point &p: points) {
        p.hash = hashing_scheme.hash(p);
    }

    unordered_map<ull, T> bucket_values;
    for (tagged_point &p: points) {
        if (bucket_values.find(p.hash) == bucket_values.end())
            bucket_values[p.hash] = f.empty_value;
        bucket_values[p.hash] = f.compose(bucket_values[p.hash], f.evaluate(p));
    }

    vector<T> proximity_points(points.size(), f.empty_value);
    for (int point_i=0; point_i<(int) points.size(); point_i++) {
        proximity_points[point_i] = hashing_scheme.eval_ball(points[point_i], radius, f, bucket_values);
    }

    return proximity_points;
}

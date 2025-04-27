#include <vector>

#include "points.hpp"
#include "hashing.hpp"

template<typename T>
std::vector<T> eval_composable(
    int dim,
    std::vector<tagged_point>& points,
    double radius,
    const Composable::Composable<T>& f,
    HashingSchemeChoice hs_choice
) {
    std::unique_ptr<HashingScheme<T>> hashing_scheme = make_hashing_scheme<T>(hs_choice, dim, radius * scale);

    #pragma omp parallel for
    for (tagged_point &p: points) {
        p.hash = hashing_scheme->hash(p);
    }

    std::unordered_map<ull, T> bucket_values;
    for (tagged_point &p: points) {
        if (bucket_values.find(p.hash) == bucket_values.end())
            bucket_values[p.hash] = f.empty_value;
        bucket_values[p.hash] = f.compose(bucket_values[p.hash], f.evaluate(p));
    }

    std::vector<T> proximity_points(points.size(), f.empty_value);
    #pragma omp parallel for
    for (int point_i=0; point_i<(int) points.size(); point_i++) {
        proximity_points[point_i] = hashing_scheme->eval_ball(points[point_i], radius, f, bucket_values);
    }

    return proximity_points;
}

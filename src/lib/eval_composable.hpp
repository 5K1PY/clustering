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

    std::vector<std::vector<ull>> hashes(points.size());
    #pragma omp parallel for
    for (size_t i=0; i<points.size(); i++) {
        hashes[i] = hashing_scheme->hash(points[i]);
    }

    std::unordered_map<std::vector<ull>, T, VectorULLHash> bucket_values;
    for (size_t i=0; i<points.size(); i++) {
        tagged_point &p = points[i];
        if (bucket_values.find(hashes[i]) == bucket_values.end())
            bucket_values[hashes[i]] = f.empty_value;
        bucket_values[hashes[i]] = f.compose(bucket_values[hashes[i]], f.evaluate(p));
    }

    std::vector<T> proximity_points(points.size(), f.empty_value);
    #pragma omp parallel for
    for (int point_i=0; point_i<(int) points.size(); point_i++) {
        proximity_points[point_i] = hashing_scheme->eval_ball(points[point_i], radius, f, bucket_values);
    }

    return proximity_points;
}

#include "util.hpp"
#include "points.hpp"
#include "hashing.hpp"

double get_gamma(const HashingScheme hashing_scheme, int dimension) {
    switch (hashing_scheme) {
        case GridHashingScheme: return GridHashing<point>::Gamma(dimension);
        case FaceHashingScheme: return FaceHashing<point>::Gamma(dimension);
        default: throw std::invalid_argument("Unsupported hashing scheme");
    }
}

HashingScheme choose_hashing_scheme(std::string choice) {
    if (choice == "face_hashing")      return FaceHashingScheme;
    else if (choice == "grid_hashing") return GridHashingScheme;
    else                               invalid_usage_solver();
}

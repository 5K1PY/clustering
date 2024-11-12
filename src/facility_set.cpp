#include "hashing.h"
#include "points.h"
#include "facility_set.h"

void invalid_usage() {
    cerr << "Usage: ./facility_set {face_hashing, grid_hashing} [seed]" << endl;
    exit(2);
}

int main(int argc, char const *argv[]) {
    HashingScheme hashing_scheme;
    if (argc != 3) invalid_usage();
    
    std::string hs = argv[1];
    if (hs == "face_hashing")      hashing_scheme = FaceHashingScheme;
    else if (hs == "grid_hashing") hashing_scheme = GridHashingScheme;
    else                                invalid_usage();

    seed(strtoull(argv[2], 0, 16));

    int n, dim; double facility_cost;
    cin >> n >> dim >> facility_cost;
    auto points = load_points(n, dim);

    auto chosen = compute_facilities(dim, points, facility_cost, hashing_scheme);
    for (auto c: chosen) {
        cout << c << " ";
    }
    cout << endl;
}

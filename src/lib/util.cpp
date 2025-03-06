#include <iostream>

[[noreturn]]
void invalid_usage_solver() {
    std::cerr << "Usage: ./facility_set {face_hashing, grid_hashing} [seed]" << std::endl;
    exit(2);
}

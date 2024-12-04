#pragma once

#include <iostream>

using namespace std;

[[noreturn]]
void invalid_usage_solver() {
    cerr << "Usage: ./facility_set {face_hashing, grid_hashing} [seed]" << endl;
    exit(2);
}

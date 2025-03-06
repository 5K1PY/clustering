#include "bin_search_unittests.hpp"
#include "hashing_unittests.hpp"
#include "points_unittests.hpp"

#include "gtest/gtest.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

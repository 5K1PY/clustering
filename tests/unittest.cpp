#include "bin_search_unittests.h"
#include "hashing_unittests.h"
#include "points_unittests.h"

#include "gtest/gtest.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

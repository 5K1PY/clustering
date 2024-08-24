#pragma once
#include "../src/hashing.h"

#include "gtest/gtest.h"

TEST(GridHashing, NoOffset) {
    ull cell_size = 10;
    ull offset_zero = numeric_limits<ll>::min();
    GridHashing gh(2, cell_size, {offset_zero, offset_zero});

    point p1({0, 0});
    ull p1_hash = gh.hash(p1); 
    point p2({0, (double) cell_size / scale});
    ull p2_hash = gh.hash(p2);
    point p3({(double) cell_size / scale, 0});
    ull p3_hash = gh.hash(p3);

    ASSERT_EQ(p1_hash, 0);
    ASSERT_EQ(p2_hash, 1);
    ASSERT_EQ(p3_hash, gh.hash_poly() % gh.hash_mod());

    // Test bucket edge
    point p4({0, ((double) cell_size - 0.00001) / scale});
    ull p4_hash = gh.hash(p4);
    ASSERT_NE(p2_hash, p4_hash);
}

TEST(GridHashing, RandomOffset) {
    ull cell_size = 10;
    GridHashing gh(2, 10);

    point p1({0, 0});
    ull p1_hash = gh.hash(p1); 
    point p2({0, (double) cell_size / scale});
    ull p2_hash = gh.hash(p2);
    point p3({(double) cell_size / scale, 0});
    ull p3_hash = gh.hash(p3);

    ASSERT_NE(p1_hash, p2_hash);
    ASSERT_NE(p1_hash, p3_hash);
    ASSERT_NE(p2_hash, p3_hash);
}

TEST(GridHashing, BucketSphereIntersect) {
    int dim = 2;
    ull cell_size = 10;
    ull offset_zero = numeric_limits<ll>::min();
    GridHashing gh(dim, cell_size, {offset_zero, offset_zero});

    double cs_half = cell_size / 2.0 / scale;
    double epsilon = sqrt(dim) / scale;

    point bucket({cs_half, cs_half});
    point p1({3*cs_half, cs_half});
    point p2({cs_half, -cs_half});
    point p3({3*cs_half, 3*cs_half});

    ASSERT_FALSE(gh.bucket_sphere_intersect(p1, cs_half - epsilon, bucket));
    ASSERT_TRUE(gh.bucket_sphere_intersect(p1, cs_half + epsilon, bucket));

    ASSERT_FALSE(gh.bucket_sphere_intersect(p2, cs_half - epsilon, bucket));
    ASSERT_TRUE(gh.bucket_sphere_intersect(p2, cs_half + epsilon, bucket));

    ASSERT_FALSE(gh.bucket_sphere_intersect(p3, sqrt(2.0) * cs_half - epsilon, bucket));
    ASSERT_TRUE(gh.bucket_sphere_intersect(p3, sqrt(2.0) * cs_half + epsilon, bucket));
}

#pragma once
#include "../src/points.h"

#include "gtest/gtest.h"

TEST(Points, Equality) {
    point origin({0, 0}), origin2({0, 0}), p1({0, 1});
    ASSERT_EQ(origin, origin2);
    ASSERT_NE(origin, p1);
}

TEST(Points, Addition) {
    point origin({0, 0}), p1({0, 1}), p2({1, 0}), p3({1, 1});
    ASSERT_EQ(p1 + origin, p1);
    ASSERT_EQ(p1 + p2, p2 + p1);
    ASSERT_EQ(p1 + p2, p3);
}

TEST(Points, Distance) {
    point origin({0, 0}), p1({0, 1}), p2({1, 0}), p3({1, 1});
    ASSERT_EQ(origin.dist_squared(p1), 1.0);
    ASSERT_EQ(origin.dist_squared(p2), 1.0);
    ASSERT_EQ(origin.dist_squared(p3), 2.0);
    ASSERT_EQ(p1.dist(p2), sqrt(2.0));
}

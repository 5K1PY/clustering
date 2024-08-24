#pragma once
#include "../src/points.h"

#include "gtest/gtest.h"

TEST(Points, Equality) {
    point p({0, 0});
    point p2({0, 0});
    ASSERT_EQ(p, p2);
}

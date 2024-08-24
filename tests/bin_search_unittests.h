#pragma once
#include "../src/bin_search.h"

#include "gtest/gtest.h"

TEST(BinSearch, Integers) {
    ASSERT_EQ(binary_search<int>([](int x){return x >= 4;}, 0, 10), 4);
    ASSERT_EQ(binary_search<int>([](int x){return x >= 5;}, 0, 10), 5);
    ASSERT_EQ(binary_search<int>([](int x){return x >= 6;}, 0, 10), 6);
    ASSERT_EQ(binary_search<int>([](int x){return x >= 7;}, 0, 10), 7);
    ASSERT_EQ(binary_search<int>([](int x){return x >= 8;}, 0, 10), 8);
}

TEST(BinSearch, Array) {
    int arr[] = {0, 3, 5, 8, 12};
    ASSERT_EQ(binary_search<int>([&arr](int i){return arr[i] >= arr[0];}, 0, 5), 0);
    ASSERT_EQ(binary_search<int>([&arr](int i){return arr[i] >= arr[1];}, 0, 5), 1);
    ASSERT_EQ(binary_search<int>([&arr](int i){return arr[i] >= arr[2];}, 0, 5), 2);
    ASSERT_EQ(binary_search<int>([&arr](int i){return arr[i] >= arr[3];}, 0, 5), 3);
    ASSERT_EQ(binary_search<int>([&arr](int i){return arr[i] >= arr[4];}, 0, 5), 4);
}

TEST(BinSearch, IntegersNoUpLimit) {
    ASSERT_EQ(binary_search<int>([](int x){return x >= 4;}, 0), 4);
    ASSERT_EQ(binary_search<int>([](int x){return x >= 5;}, 0), 5);
    ASSERT_EQ(binary_search<int>([](int x){return x >= 6;}, 0), 6);
    ASSERT_EQ(binary_search<int>([](int x){return x >= 7;}, 0), 7);
    ASSERT_EQ(binary_search<int>([](int x){return x >= 8;}, 0), 8);
}

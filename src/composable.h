#pragma once

#include <queue>
#include <unordered_set>
#include <unordered_map>

#include "hashing.h"
#include "points.h"
#include "r_p.h"

using namespace std;

namespace Composable {
    template<typename T>
    struct Composable {
        Composable() {}
        ~Composable() = default;

        T empty_value;
        virtual T evaluate(const tagged_point& p) = 0;
        virtual T compose(T val1, T val2) = 0;
    };

    struct __Size : Composable<int> {
        int empty_value = 0;
        int evaluate(const tagged_point& p) override {
            return 1;
        }
        int compose(int val1, int val2) override {
            return val1 + val2;
        }
    };

    struct __MinLabel : Composable<const tagged_point*> {
        const tagged_point* empty_value = NULL;
        const tagged_point* evaluate(const tagged_point& p) override {
            return &p;
        }
        const tagged_point* compose(const tagged_point* val1, const tagged_point* val2) override {
            if (val1 == NULL) return val2;
            if (val2 == NULL) return val1;
            return (val1->label <= val2->label) ? val1 : val2;
        }
    };

    __Size Size = __Size();
    __MinLabel MinLabel = __MinLabel();
}

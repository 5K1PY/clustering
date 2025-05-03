#pragma once

#include "points.hpp"

namespace Composable {

    /**
     * @brief Base struct for composable function.
     *
     * @tparam T The type of the result of composable function.
     */
    template<typename T>
    struct Composable {
        Composable() {}
        virtual ~Composable() = default;

        T empty_value; ///< Result of the composable function on empty set.

        /**
         * @brief Evaluates the function on a set with single point.
         *        (Use compose to get results for sets of greater sizes.)
         * @param p The point iside the set.
         * @return The result of the function - f({p}).
         */
        virtual T evaluate(const tagged_point& p) const = 0;

        /**
         * @brief Composes two function values.
         * @param val1 The first function value - f(S_1).
         * @param val2 The second function value - f(S_2).
         * @return The result of the composition - f(S_1 ∪ S_2).
         */
        virtual T compose(T val1, T val2) const = 0;
    };

    /**
     * @brief Size of a set of points as a composable function
     */
    struct __Size : Composable<int> {
        int empty_value = 0;
        int evaluate(const tagged_point& p) const override {
            return 1;
        }
        int compose(int val1, int val2) const override {
            return val1 + val2;
        }
    };

    /**
     * @brief Minimum label in a set of points as a composable function
     */
    struct __MinLabel : Composable<const tagged_point*> {
        const tagged_point* empty_value = NULL;
        const tagged_point* evaluate(const tagged_point& p) const override {
            return &p;
        }
        const tagged_point* compose(const tagged_point* val1, const tagged_point* val2) const override {
            if (val1 == NULL) return val2;
            if (val2 == NULL) return val1;
            return (val1->label <= val2->label) ? val1 : val2;
        }
    };

    /// Singleton instance of the __Size composable function.
    extern __Size Size;
    /// Singleton instance of the __MinLabel composable function.
    extern __MinLabel MinLabel;
}

#pragma once

#include "includes.hpp"

template<typename U>
class LAGRANGE_: public SimpleSearch<scalar_of_t<U>>, protected IMethod<scalar_of_t<U>>{
private:
    using T = scalar_of_t<U>;
    USING_ALL(T)

    static T function_target(T x, T y) {
        return x * x + y * y;
    }

    static T constraint(T x, T y) {
        return x + y - 1;
    }

    static void gradient(T x, T y, T lambda, T r, T& gx, T& gy) {
        T h = constraint(x, y);
        gx = 2 * x + lambda + r * h;
        gy = 2 * y + lambda + r * h;
    }

    static void augmented_lagrangian(T& x, T& y) {
        T lambda = 0.0;
        T r      = 10.0;
        T alpha  = 0.01;

        for (int k = 0; k < 10; k++) {
            for (int i = 0; i < 500; i++) {
                T gx, gy;
                gradient(x, y, lambda, r, gx, gy);
                x -= alpha * gx;
                y -= alpha * gy;
            }

            lambda += r * constraint(x, y);

            LOG_METHOD(function_target(x, y));
        }
    }

public:
    static T f(type_var_foo target_f, T a, T b) {
        (void)target_f;
        REPACK(target_f, T,
            (void)target_f_;
            T x = a;
            T y = b;
            augmented_lagrangian(x, y);
            return function_target(x, y);
        );
    }
    REGISTER_FUNCTION
};

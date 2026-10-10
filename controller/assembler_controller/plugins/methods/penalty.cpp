#pragma once

#include "includes.hpp"

template<typename U>
class PENALTY_: public SimpleSearch<scalar_of_t<U>>, protected IMethod<scalar_of_t<U>>{
private:
    using T = scalar_of_t<U>;
    USING_ALL(T)

    static T function_target(T x, T y) {
        return x * x + y * y;
    }

    static T penalty(T x, T y) {
        T violation = 1 - x - y;
        if (violation > 0)
            return violation * violation;
        return 0.0;
    }

    static T penalty_function(T x, T y, T r) {
        return function_target(x, y) + r * penalty(x, y);
    }

    static void gradient(T x, T y, T r, T& gx, T& gy) {
        gx = 2 * x;
        gy = 2 * y;
        T violation = 1 - x - y;
        if (violation > 0) {
            gx -= 2 * r * violation;
            gy -= 2 * r * violation;
        }
    }

    static void penalty_method(T& x, T& y) {
        T r     = 1;
        T beta  = 10;
        T alpha = 0.01;

        for (int k = 0; k < 5; k++) {
            for (int i = 0; i < 1000; i++) {
                T gx, gy;
                gradient(x, y, r, gx, gy);
                x -= alpha * gx;
                y -= alpha * gy;
            }

            r *= beta;

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
            penalty_method(x, y);
            return function_target(x, y);
        );
    }
    REGISTER_FUNCTION
};

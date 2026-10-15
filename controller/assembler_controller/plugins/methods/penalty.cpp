#pragma once
#include "includes.hpp"

template<typename U>
class PENALTY_: public SimpleSearch<scalar_of_t<U>>,
                protected IMethod<scalar_of_t<U>, U>{
private:
    using T = scalar_of_t<U>;
    USING_ALL(T, U)
    using Point = std::vector<T>;

public:
    static T f(type_var_foo target_f, U arg){
        REPACK(target_f, T,
            auto eval = [&](const Point& p) -> T {
                return get_result_foo(target_f_, p);
            };
            auto pen  = [](T x, T y){ T v = 1 - x - y; return v > 0 ? v*v : T(0); };
            auto grad_at = [&](const Point& p, T r) -> Point {
                const T h = static_cast<T>(1e-6);
                T x = p[0], y = p[1];
                Point g(2);
                g[0] = (eval(Point{x+h, y}) - eval(p)) / h + 2*r*(1-x-y > 0 ? (x+y-1) : 0);
                g[1] = (eval(Point{x, y+h}) - eval(p)) / h + 2*r*(1-x-y > 0 ? (x+y-1) : 0);
                return g;
            };

            T x = static_cast<T>(arg[0]);
            T y = static_cast<T>(arg[1]);
            T r = 1, beta = 10, alpha = 0.01;

            FOR(0, 5, {
                FOR(0, 1000, {
                    Point g = grad_at(Point{x, y}, r);
                    x -= alpha * g[0];
                    y -= alpha * g[1];
                });
                r *= beta;
                LOG_METHOD(eval(Point{x, y}));
            });
            return eval(Point{x, y});
        );
    }
    REGISTER_FUNCTION
};

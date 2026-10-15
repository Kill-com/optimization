#pragma once
#include "includes.hpp"

template<typename U>
class BARRIER_: public SimpleSearch<scalar_of_t<U>>,
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
            auto grad_at = [&](const Point& p) -> Point {
                const T h = static_cast<T>(1e-6);
                T f0 = eval(p);
                Point g(2);
                Point pp = p; pp[0] += h; g[0] = (eval(pp) - f0) / h;
                pp = p;       pp[1] += h; g[1] = (eval(pp) - f0) / h;
                return g;
            };

            T x = static_cast<T>(arg[0]);
            T y = static_cast<T>(arg[1]);
            T mu = 1, beta = 0.5, alpha = 0.01;

            FOR(0, 10, {
                FOR(0, 500, {
                    Point g = grad_at(Point{x, y});
                    T t = x + y - 1;
                    x -= alpha * (g[0] - mu / t);
                    y -= alpha * (g[1] - mu / t);
                });
                LOG_METHOD(eval(Point{x, y}));
                mu *= beta;
            });
            return eval(Point{x, y});
        );
    }
    REGISTER_FUNCTION
};

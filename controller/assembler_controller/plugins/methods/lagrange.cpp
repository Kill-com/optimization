#pragma once
#include "includes.hpp"

template<typename U>
class LAGRANGE_: public SimpleSearch<scalar_of_t<U>>,
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
            auto h = [](T x, T y) { return x + y - 1; };
            auto grad_at = [&](const Point& p, T lambda, T r) -> Point {
                const T eps = static_cast<T>(1e-6);
                T x = p[0], y = p[1];
                T hp = h(x, y);
                Point g(2);
                Point px = {x + eps, y};
                Point py = {x, y + eps};
                g[0] = (eval(px) - eval(p)) / eps + lambda + r * hp;
                g[1] = (eval(py) - eval(p)) / eps + lambda + r * hp;
                return g;
            };

            T x = static_cast<T>(arg[0]);
            T y = static_cast<T>(arg[1]);
            T lambda = 0, r = 10, alpha = 0.01;

            FOR(0, 10, {
                FOR(0, 500, {
                    Point g = grad_at(Point{x, y}, lambda, r);
                    x -= alpha * g[0];
                    y -= alpha * g[1];
                });
                lambda += r * h(x, y);
                LOG_METHOD(eval(Point{x, y}));
            });
            return eval(Point{x, y});
        );
    }
    REGISTER_FUNCTION
};

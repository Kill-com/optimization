#pragma once
#include "includes.hpp"

template<typename U>
class GRAD_SPYSK_: public SimpleSearch<scalar_of_t<U>>,
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

            Point x = {static_cast<T>(arg[0]), static_cast<T>(arg[1])};

            FOR(0, 1000, {
                Point g = grad_at(x);
                Point d = {-g[0], -g[1]};

                T fx  = eval(x);
                T gtd = g[0]*d[0] + g[1]*d[1];
                T alpha = 1;
                Point xn = {x[0] + alpha*d[0], x[1] + alpha*d[1]};
                WHILE(0, [&](int){
                    return alpha > static_cast<T>(1e-12)
                        && eval(xn) > fx + static_cast<T>(1e-4)*alpha*gtd;
                }, {
                    alpha *= 0.5;
                    xn = {x[0] + alpha*d[0], x[1] + alpha*d[1]};
                });

                x = xn;
                LOG_METHOD(eval(x));
            });
            return eval(x);
        );
    }
    REGISTER_FUNCTION
};

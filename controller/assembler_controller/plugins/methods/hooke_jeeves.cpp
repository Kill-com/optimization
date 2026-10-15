#pragma once
#include "includes.hpp"

template<typename U>
class HOOKE_JEEVES_: public SimpleSearch<scalar_of_t<U>>,
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

            T x = static_cast<T>(arg[0]);
            T y = static_cast<T>(arg[1]);
            T alpha = 1, eps = 0.001, gamma = 0.5;

            WHILE(0, [&](int){ return alpha > eps; }, {
                bool improved = false;
                T bestX = x, bestY = y;
                T bestV = eval(Point{x, y});

                if (eval(Point{x + alpha, y}) < bestV) { bestX = x + alpha; bestV = eval(Point{bestX, y}); improved = true; }
                if (eval(Point{x - alpha, y}) < bestV) { bestX = x - alpha; bestV = eval(Point{bestX, y}); improved = true; }
                if (eval(Point{bestX, y + alpha}) < bestV) { bestY = y + alpha; bestV = eval(Point{bestX, bestY}); improved = true; }
                if (eval(Point{bestX, y - alpha}) < bestV) { bestY = y - alpha; bestV = eval(Point{bestX, bestY}); improved = true; }

                x = bestX; y = bestY;
                LOG_METHOD(eval(Point{x, y}));
                if (!improved) alpha *= gamma;
            });
            return eval(Point{x, y});
        );
    }
    REGISTER_FUNCTION
};

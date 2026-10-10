#pragma once

#include "includes.hpp"

template<typename U>
class HOOKE_JEEVES_: public SimpleSearch<scalar_of_t<U>>, protected IMethod<scalar_of_t<U>>{
private:
    using T = scalar_of_t<U>;
    USING_ALL(T)

    static T f_target(T x, T y) {
        return std::pow(x - 2, 2) + std::pow(y - 3, 2);
    }

    static void hooke_jeeves(T& x, T& y) {
        T alpha = 1.0;
        T eps   = 0.001;
        T gamma = 0.5;
        int iter = 0;

        while (alpha > eps) {
            bool improved = false;
            T bestX = x;
            T bestY = y;
            T bestValue = f_target(x, y);

            if (f_target(x + alpha, y) < bestValue) {
                bestX = x + alpha;
                bestValue = f_target(bestX, y);
                improved = true;
            }
            if (f_target(x - alpha, y) < bestValue) {
                bestX = x - alpha;
                bestValue = f_target(bestX, y);
                improved = true;
            }

            if (f_target(bestX, y + alpha) < bestValue) {
                bestY = y + alpha;
                bestValue = f_target(bestX, bestY);
                improved = true;
            }

            if (f_target(bestX, y - alpha) < bestValue) {
                bestY = y - alpha;
                bestValue = f_target(bestX, bestY);
                improved = true;
            }

            x = bestX;
            y = bestY;

            LOG_METHOD(f_target(x, y));

            iter++;

            if (!improved)
                alpha *= gamma;
        }
    }

public:
    static T f(type_var_foo target_f, T a, T b) {
        (void)target_f;
        REPACK(target_f, T,
            (void)target_f_;
            T x = a;
            T y = b;
            hooke_jeeves(x, y);
            return f_target(x, y);
        );
    }
    REGISTER_FUNCTION
};

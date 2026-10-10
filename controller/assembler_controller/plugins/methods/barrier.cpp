#pragma once

#include "includes.hpp"

template<typename U>
class BARRIER_:public SimpleSearch<scalar_of_t<U>>, protected IMethod<scalar_of_t<U>>{
private:
    using T=scalar_of_t<U>;
    USING_ALL(T)

    static T function_target(T x, T y){
        return x*x + y*y;
    }

    static T barrier_function(T x, T y, T mu){
        return function_target(x, y) - mu * log(x + y - 1);
    }

    static void gradient(T x, T y, T mu, T& gx, T& gy){
        T t = x + y - 1;
        gx = 2*x - mu / t;
        gy = 2*y - mu / t;
    }

public:
    static T f(type_var_foo target_f, T a, T b){
        (void)target_f;
        REPACK(target_f, T,
            (void)target_f_;

            T x = a;
            T y = b;

            T mu    = 1.0;
            T beta  = 0.5;
            T alpha = 0.01;

            for(int k = 0; k < 10; k++){

                for(int i = 0; i < 500; i++){
                    T gx, gy;
                    gradient(x, y, mu, gx, gy);
                    x -= alpha * gx;
                    y -= alpha * gy;
                }

                LOG_METHOD(function_target(x, y));
            }

            return function_target(x, y);
        );
    }
    REGISTER_FUNCTION
};

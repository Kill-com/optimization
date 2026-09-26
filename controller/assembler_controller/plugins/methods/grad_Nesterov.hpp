#pragma once


// СКОРЕЕ ВСЕГО НЕ РАБОТАЕТ
#include <cmath>
#include <iostream>
#include <functional>
#include <vector>

#include "hepler_methods/gradient_calc.hpp"
#include "includes.hpp"

template<typename T>
class GRADNESTEROV_:public EPSContainer<T>, public BContainer<T>,
    public IMethod<scalar_of_t<T>,T>{
    using EPSContainer<T>::EPS;
    using BContainer<T>::B;
    using U =scalar_of_t<T>;
    USING_ALL(U,T)
public:
    static T f(type_var_foo func,
        std::function<T(type_var_foo, T)> gradient_step,
        std::function<U(std::function<U(U)>, U,U)> simp_search,
        std::function<std::pair<U, U>(std::function<U(U)>)> bracket, std::vector<U> arg){
        size_t n = arg.size();
        T v(n, 0.0);
        T grad(n);
        T nest_arg(n);
        std::function<U(U)> func_one;
        U a;
        U grad_norm = 1;
        std::pair<U,U> p;
        REPACK(func, T,
            WHILE(0, [&](int i){return grad_norm > EPS;},{
                FOR(0, n,{
                    nest_arg[i] = arg[i] + B*v[i];
                });
                grad = gradient_step(func_, arg);
                grad_norm = gradient_norm<U>(grad);
        
                func_one = [&](U t){
                    T arg_one_per = arg;
                    FOR(0, arg.size(),{
                        arg_one_per[i] = arg_one_per[i] - grad[i] / grad_norm * t;
                    });
                    return get_result_foo(func_,arg_one_per);
                };
                // Поиск шага методом золотого сечения
                p=bracket(func_one);
                a = simp_search(func_one,p.first,p.second);
                
                FOR(0, n,{
                    v[i] = B * v[i] - a * grad[i]/grad_norm;
                    arg[i] = arg[i] + v[i];
                });
                LOG_METHOD(arg)
            });
            LOG_METHOD(arg)
            return arg;
        )
    }
    REGISTER_FUNCTION
};

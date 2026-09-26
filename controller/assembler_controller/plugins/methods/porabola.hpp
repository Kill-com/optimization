#pragma once

#include "includes.hpp"

template<typename U>
class PORABOLA_:public SimpleSearch<scalar_of_t<U>>,protected IMethod<scalar_of_t<U>>{
private:
    using T=scalar_of_t<U>; 
    using EPSContainer<T>::EPS;
    USING_ALL(T)
public:
    static T f(type_var_foo target_f, T a, T c) {
        REPACK(target_f,T,
        T b = (c + a) / 2;
        T x = b;          
        T b_old = b;   

        // Используем WHILE с поддержкой BREAK
        WHILE (0,
            [&](T) { 
                return (c - a) > EPS || std::abs(get_result_foo(target_f_,b) - get_result_foo(target_f_,b_old)) > EPS; 
            },
            {
                b_old = b;       
                T znam = (c - b) * get_result_foo(target_f_,a) + (a - c) * get_result_foo(target_f_,b) + (b - a) * get_result_foo(target_f_,c);
                
                if (std::abs(znam) < 1e-12) {
                    BREAK;  // ← Выход из цикла
                } else {
                    x = -0.5 * (((get_result_foo(target_f_,b) - get_result_foo(target_f_,a)) * (c - a) * (c - b) -
                                (a + b) * ((c - b) * get_result_foo(target_f_,a) + (a - c) * get_result_foo(target_f_,b) + (b - a) * get_result_foo(target_f_,c))) / znam);

                    if (x <= a || x >= c) {
                        BREAK;  // ← Выход из цикла
                    } else {
                        if (get_result_foo(target_f_ , x) < get_result_foo(target_f_, b)) {
                            if (x < b) {
                                c = b;
                                b = x;
                            } else { 
                                a = b;
                                b = x;
                            }
                        } else {
                            if (x < b) {
                                a = x;
                            } else { 
                                c = x;
                            }
                        }
                    }
                }
                LOG_METHOD(b)
            }
        );
        return b;
    )
    }
    REGISTER_FUNCTION
};

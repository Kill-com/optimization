#pragma once

#include <vector>
#include <cmath>

#include "../includes.hpp"

template<typename T>
class ISTEP:public EPSContainer<T>, public IMethod<scalar_of_t<T>,T>{
protected:
    using U=scalar_of_t<T>;
    using typename IMethod<U,T>::type_var_foo;
    using IMethod<U,T>::get_result_foo;
};

template<typename T>
class FIRSTSTEP_: public ISTEP<T>{
    using U=scalar_of_t<T>;
    using typename ISTEP<T>::type_var_foo;
    using ISTEP<T>::get_result_foo;
    using ISTEP<T>::EPS;
public:
    static T f(type_var_foo func, T arg) {
        T grad(arg.size());
        U f_plus=0;
        REPACK(func, T,
            scalar_of_t<T> f = get_result_foo(func_,arg);
            FOR(0,arg.size(),{
                arg[i] += EPS;
                f_plus = get_result_foo(func_,arg);
                
                arg[i] -= EPS;
                
                grad[i] = (f_plus - f) / EPS;
            });
            return grad;
        )
    }
    REGISTER_FUNCTION
};

template<typename T>
class SECONDSTEP_: public ISTEP<T>{
    using typename ISTEP<T>::type_var_foo;
    using ISTEP<T>::get_result_foo;
    using ISTEP<T>::EPS;
    using U=scalar_of_t<T>;
public:
    static T f(type_var_foo func, T arg) {
        T grad(arg.size());
        U f_plus=0;
        U f_minus=0;
        REPACK(func, T,
            FOR(0, arg.size(),{
                                
                arg[i] += EPS;
                f_plus = get_result_foo(func_,arg);
                
                arg[i] -= 2 * EPS;
                f_minus = get_result_foo(func_,arg);
    
                arg[i] += EPS;
                
                grad[i] = (f_plus - f_minus) / (2 * EPS);
            });
            f_plus=0;
            f_minus=0;
            return grad;
        )
    }
    REGISTER_FUNCTION
};

template<typename T>
T gradient_norm(std::vector<T> grad) {
    T grad_norm = 0;
    for(std::size_t i = 0; i<grad.size(); i++){
        grad_norm += grad[i]*grad[i];
    }
    grad_norm = sqrt(grad_norm);
    return grad_norm;
}
#pragma once

#include <vector>
#include <cmath>

template<typename T, typename Func>
std::vector<T> gradient_first_step(Func func, std::vector<T> arg) {

    T delta_arg = static_cast<T>(1e-6);
    std::vector<T> grad(arg.size());
    T f = func(arg);

    for(std::size_t i = 0; i < arg.size(); i++){
        
        arg[i] += delta_arg;
        T f_plus = func(arg);
        
        arg[i] -= delta_arg;
        
        grad[i] = (f_plus - f) / delta_arg;
    }
    return grad;
}


template<typename T, typename Func>
std::vector<T> gradient_second_step(Func func, std::vector<T> arg) {
    T delta_arg = static_cast<T>(1e-6);
    std::vector<T> grad(arg.size());
    for(std::size_t i = 0; i < arg.size(); i++){
        
        arg[i] += delta_arg;
        T f_plus = func(arg);
        
        arg[i] -= 2 * delta_arg;
        T f_minus = func(arg);

        arg[i] += delta_arg;
        
        grad[i] = (f_plus - f_minus) / (2 * delta_arg);
    }
    return grad;
}

template<typename T>
T gradient_norm(std::vector<T> grad) {
    T grad_norm = 0;
    for(std::size_t i = 0; i<grad.size(); i++){
        grad_norm += grad[i]*grad[i];
    }
    grad_norm = sqrt(grad_norm);
    return grad_norm;
}
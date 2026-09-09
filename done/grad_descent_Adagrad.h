
#include <cmath>
#include <iostream>
#include <functional>
#include <vector>

#include "done/bracket_interval.h"
#include "done/search_min.h"
#include "done/gradient_calc.h"





template<typename T>
std::vector<T> grad_descent_Adagrad(std::function<T(std::vector<T>)>& func, std::vector<T> arg){

    size_t n = arg.size();

    if(n == 0) return {};

    T ideal = 1e-4; // условие сходимости 
    T e = 1e-8;
    T grad_norm = 1;

    std::vector<T> s(n, 0.0);

    do{

        std::vector<T> grad = gradient_first_step<T>(func, arg);

        grad_norm = gradient_norm<T>(grad);

        std::function<T(T)> func_one = [&func, &arg, &grad, &grad_norm](T t){
            std::vector<T> arg_one_per = arg;
            for(std::size_t i = 0; i < arg.size(); i++){
                arg_one_per[i] = arg_one_per[i] - grad[i] / grad_norm * t;
            }
            return func(arg_one_per);
        };

        for(size_t i = 0; i<n; i++){
            s[i] += grad[i]*grad[i];
        }

        T a = golden_section<T>(func_one, bracket_phi<T>(func_one));

        for(size_t i = 0; i<n; i++){
            arg[i] = arg[i] - (a*grad[i])/(e + sqrt(s[i]));
        }

    }while(grad_norm > ideal && iteration < 10000);
    return arg;
}
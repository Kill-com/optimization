
#include <cmath>
#include <iostream>
#include <functional>
#include <vector>

#include "done/bracket_interval.h"
#include "done/search_min.h"
#include "done/gradient_calc.h"



template <typename T>
T grad_descent_Nesterov(std::function<T(std::vector<T>)>& func, std::vector<T> arg){

    size_t n = arg.size();

    if(n == 0) return 0;

    T b = static_cast<T>(0.9); // скорость затухания инерции "шарика)"
    T ideal = static_cast<T>(1e-6); // условие сходимости 

    std::vector<T> v(n, 0.0);

    do{
        std::vector<std::vector<T>> grad = gradient_first_step<T>(func, arg);

        std::vector<T> nest_arg(n);

        for(size_t i = 0; i<n; i++){
            nest_arg[i] = arg[i] + b*v[i];
        }

        std::vector<std::vector<T>> gradN = gradient_first_step<T>(func, nest_arg);


        T grad_norm = gradient_norm<T>(grad);


        for(int i = 0; i<n; i++){
            v[i] = b * v[i] - a * gradS[i];
            arg[i] = arg[i] + v[i];
        }


    }while(grad_norm > ideal);
    return arg;
}

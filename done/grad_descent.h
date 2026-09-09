#pragma once

#include <cmath>
#include <iostream>
#include <functional>
#include <vector>

#include "bracket_interval.h"
#include "search_min.h"
#include "gradient_calc.h"



template<typename A>
std::vector<A> grad_descent(const std::function<A(std::vector<A>)>& func, std::vector<A> arg){


    std::size_t n = arg.size();

    if(n == 0) return {};

    A ideal = static_cast<A>(1e-4); // условие сходимости

    A grad_norm = 1;

    do{
        

        std::vector<A> grad = gradient_first_step(func, arg);

        grad_norm = gradient_norm(grad);
        
        if(grad_norm < ideal) break;

        std::function<A(A)> func_one = [&func, &arg, &grad, &grad_norm](A t){
            std::vector<A> arg_one_per = arg;
            for(std::size_t i = 0; i<arg.size(); i++) {arg_one_per[i] = arg_one_per[i] - grad[i]/grad_norm * t;}
            return func(arg_one_per);
        };

        
        A a = golden_section(func_one, bracket_geom(func_one));
        

        for(std::size_t i = 0; i<n; i++){
            arg[i] = arg[i] - a*grad[i]/grad_norm;
        }

    }while(grad_norm > ideal);
    return arg;
}

#include <cmath>
#include <iostream>
#include <functional>
#include <vector>



template<typename A>
std::vector<A> grad_descent(const std::function<A(std::vector<A>)>& func, std::vector<A> arg){


    std::size_t n = arg.size();

    if(n == 0) return {};

    A delta_arg = static_cast<A>(1e-6);
    A ideal = static_cast<A>(1e-4); // условие сходимости

    A grad_norm = 1;

    do{
        

        std::vector<A> grad(n);
        std::vector<A> arg_copy(n);
        for(std::size_t i = 0; i < n; i++){
            arg_copy = arg;
            arg_copy[i] = arg_copy[i] + delta_arg;
            grad[i] = (func(arg_copy) - func(arg))/delta_arg;
        }

        grad_norm = 0;
        
        for(std::size_t i = 0; i<n; i++){
            grad_norm = grad_norm + grad[i]*grad[i];
        }

        grad_norm = sqrt(grad_norm);
        
        if(grad_norm < ideal) break;

        std::function<A(A)> func_one = [&func, &arg, &grad, &grad_norm](A t){
            std::vector<A> arg_one_per = arg;
            for(std::size_t i = 0; i<arg.size(); i++) {arg_one_per[i] = arg_one_per[i] - grad[i]/grad_norm * t;}
            return func(arg_one_per);
        };

        A a = static_cast<A>(0);
        A t0 = static_cast<A>(0);
        A h = static_cast<A>(0.01);               
        A t1 = t0 + h;
        A f0 = func_one(t0);
        A f1 = func_one(t1);

        if (f1 > f0) {
            h = -h;
            std::swap(t0, t1);
            std::swap(f0, f1);
        }

        A t2 = t1 + h;
        A f2 = func_one(t2);

        while (f2 < f1 && std::abs(t2) < 1e6) {
            h *= 2;
            t0 = t1;  f0 = f1;
            t1 = t2;  f1 = f2;
            t2 = t1 + h;
            f2 = func_one(t2);
        }

        if (t0 > t2) std::swap(t0, t2);
        a = golden_section(func_one, t0, t2);
        

        for(std::size_t i = 0; i<n; i++){
            arg[i] = arg[i] - a*grad[i]/grad_norm;
        }

    }while(grad_norm > ideal);
    return arg;
}
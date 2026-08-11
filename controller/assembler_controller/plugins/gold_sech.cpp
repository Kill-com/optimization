
#define _USE_MATH_DEFINES 
#include <functional>
#include <cmath>
#include <iostream>
// #include <memory>



namespace gold_sech_name{
const float EPS = 1e-6;
const float e = M_E; 
const double TAU = (std::sqrt(5.0) - 1.0) / 2.0; 
[[maybe_unused]] static float target_f(float x) {
    return std::pow((x-2), 2) + std::sin(x);
}


template<typename T>
T f(std::function<T(T)> target_f, T a, T b) {
    T x1 = a + (1-TAU)*(b-a);  
    T x2 = a + TAU*(b-a);
    std::cout<<"a="<<a<<", x1="<< x1<< ", x2="<< x2<<", b="<<b<< std::endl;

    while ((b - a) > EPS) {
        T x1 = a + (1-TAU)*(b-a);  
        T x2 = a + TAU*(b-a);
        if (target_f(x1) < target_f(x2)){
            b = x2;
            x2 = x1;
            x1 = a + (1-TAU)*(x2-a);            
        }
        else{
            a = x1;
            x1 = x2;
            x2 = a + TAU*(b-a);  
        }
        std::cout<<"a="<<a<<", x1="<< x1<< ", x2="<< x2<<", b="<<b<< std::endl;
    } 

    return(a+ b)/2;
}
}

template<typename T>
auto gold_sech(){
    return &gold_sech_name::f<T>;
}

// float(*gold_sech())(std::function<float(float)> target_f, float a, float b){
//     return &gold_sech_name::f;
// }



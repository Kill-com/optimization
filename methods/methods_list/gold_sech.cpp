#define _USE_MATH_DEFINES 
#include <functional>
#include <cmath>
#include <iostream>

const float EPS = 1e-6;
const float e = M_E; 
const double TAU = (std::sqrt(5.0) - 1.0) / 2.0; 

float target_f(float x) {
    return std::pow((x-2), 2) + std::sin(x);
}
extern "C" {
float f(std::function<float(float)> target_f, float a, float b) {
    float x1 = a + (1-TAU)*(b-a);  
    float x2 = a + TAU*(b-a);
    std::cout<<"a="<<a<<", x1="<< x1<< ", x2="<< x2<<", b="<<b<< std::endl;

    while ((b - a) > EPS) {
        float x1 = a + (1-TAU)*(b-a);  
        float x2 = a + TAU*(b-a);
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
int main() {
    float res_x = f(target_f, -10, 10);
    std::cout << "res : x = " << res_x << ", y = " << target_f(res_x) << std::endl;
    return 0;
}
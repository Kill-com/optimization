#include <functional>
#include <cmath>
#include <iostream>

const float EPS = 1e-6;
const float e = M_E; 
const double TAU = (std::sqrt(5.0) - 1.0) / 2.0; 

float f(float x) {
    return std::pow((x-2), 2) + std::sin(x);
}

float Gold_Sechenie(std::function<float(float)> f, float a, float b) {
    float x1 = a + (1-TAU)*(b-a);  
    float x2 = a + TAU*(b-a);
    std::cout<<"a="<<a<<", x1="<< x1<< ", x2="<< x2<<", b="<<b<< std::endl;

    while ((b - a) > EPS) {
        float x1 = a + (1-TAU)*(b-a);  
        float x2 = a + TAU*(b-a);
        if (f(x1) < f(x2)){
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

int main() {
    float res_x = Gold_Sechenie(f, -10, 10);
    std::cout << "res : x = " << res_x << ", y = " << f(res_x) << std::endl;
    return 0;
}
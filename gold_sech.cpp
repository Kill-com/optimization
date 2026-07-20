#define _USE_MATH_DEFINES
#include <functional>
#include <cmath>
#include <iostream>
#include <numbers>          

const float EPS = 1e-6;
const float e = M_E; 
const double TAU = 0.6180339887498949;   


extern "C" {
float f(std::function<float(float)> f, float a, float b) {
    float x1 = a + (1 - TAU) * (b - a);
    float x2 = a + TAU * (b - a);
    float fx1 = f(x1);
    float fx2 =  f(x2);
    std::cout << "a=" << a << ", x1=" << x1 << ", x2=" << x2 << ", b=" << b << std::endl;

    while ((b - a) > EPS) {
        if (fx1 < fx2) {
            b = x2;
            x2 = x1;
            fx2 = fx1;
            x1 = a + (1 - TAU) * (b - a);
            fx1 = f(x1);
        } else {
            a = x1;
            x1 = x2;
            fx1 = fx2;
            x2 = a + TAU * (b - a);
            fx2 = f(x2);
        }
        std::cout << "a=" << a << ", x1=" << x1 << ", x2=" << x2 << ", b=" << b << std::endl;
    }

    return (a + b) / 2;
}
}

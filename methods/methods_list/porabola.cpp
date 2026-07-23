#define _USE_MATH_DEFINES 
#include <functional>
#include <cmath>
#include <iostream>


const float EPS = 1e-6;
const float e = M_E; 

float target_f(float x) {
    return std::pow((x-2), 2) + std::sin(x)*x;
}
extern "C" {
float f(std::function<float(float)> target_f, float a, float c) {
    float b = (c + a) / 2;
    float x = b;          
    float b_old = b;   

    do {
        b_old = b;       
        float znam = (c - b) * target_f(a) + (a - c) * target_f(b) + (b - a) * target_f(c);
        if (std::abs(znam) < 1e-12) {
            break;        // вырожденная парабола
        }

        x = -0.5 * (((target_f(b) - target_f(a)) * (c - a) * (c - b) -
                    (a + b) * ((c - b) * target_f(a) + (a - c) * target_f(b) + (b - a) * target_f(c))) / znam);

        std::cout << "x = " << x << ", target_f(x) = " << target_f(x)
                  << ", a = " << a << ", b = " << b
                  << ", target_f(b) = " << target_f(b) << ", c = " << c << "\n";

        if (x <= a || x >= c) {
            break;
        }

        if (target_f(x) < target_f(b)) {
            if (x < b) {
                c = b;
                b = x;
            } else { 
                a = b;
                b = x;
            }
        } else {
            if (x < b) {
                a = x;
            } else { 
                c = x;
            }
        }

    } while ((c - a) > EPS || fabs(target_f(b) - target_f(b_old)) > EPS);

    return b;
}
}
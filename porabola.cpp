#include <functional>
#include <cmath>
#include <iostream>

const float EPS = 1e-6;
const float e = M_E; 

float f(float x) {
    return std::pow((x-2), 2) + std::sin(x)*x;
}

float Metog_Porabola(std::function<float(float)> f, float a, float c) {
    float b = (c + a) / 2;
    float x = b;          
    float b_old = b;   

    do {
        b_old = b;       
        float znam = (c - b) * f(a) + (a - c) * f(b) + (b - a) * f(c);
        if (std::abs(znam) < 1e-12) {
            break;        // вырожденная парабола
        }

        x = -0.5 * (((f(b) - f(a)) * (c - a) * (c - b) -
                    (a + b) * ((c - b) * f(a) + (a - c) * f(b) + (b - a) * f(c))) / znam);

        std::cout << "x = " << x << ", f(x) = " << f(x)
                  << ", a = " << a << ", b = " << b
                  << ", f(b) = " << f(b) << ", c = " << c << "\n";

        if (x <= a || x >= c) {
            break;
        }

        if (f(x) < f(b)) {
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

    } while ((c - a) > EPS || fabs(f(b) - f(b_old)) > EPS);

    return b;
}

int main() {
    float res_x = Metog_Porabola(f, -10, 2);
    std::cout << "res : x = " << res_x << ", y = " << f(res_x) << std::endl;
    return 0;
}
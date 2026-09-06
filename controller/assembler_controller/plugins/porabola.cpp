#define _USE_MATH_DEFINES 
#include <functional>
#include <cmath>
#include <iostream>
#include "../../../analisis/macros.hpp"

namespace porabola_name {
    const float EPS = 1e-6;
    const float e = M_E; 

    [[maybe_unused]] static float target_f(float x) {
        return std::pow((x-2), 2) + std::sin(x)*x;
    }

    template<typename T>
    T f(std::function<T(T)> target_f, T a, T c) {
        T b = (c + a) / 2;
        T x = b;          
        T b_old = b;   

        // Используем WHILE с поддержкой BREAK
        WHILE (0,
            [&](T) { 
                return (c - a) > EPS || std::abs(target_f(b) - target_f(b_old)) > EPS; 
            },
            {
                b_old = b;       
                T znam = (c - b) * target_f(a) + (a - c) * target_f(b) + (b - a) * target_f(c);
                
                if (std::abs(znam) < 1e-12) {
                    std::cout << "Вырожденная парабола, выход из цикла\n";
                    BREAK;  // ← Выход из цикла
                } else {
                    x = -0.5 * (((target_f(b) - target_f(a)) * (c - a) * (c - b) -
                                (a + b) * ((c - b) * target_f(a) + (a - c) * target_f(b) + (b - a) * target_f(c))) / znam);

                    std::cout << "x = " << x << ", target_f(x) = " << target_f(x)
                              << ", a = " << a << ", b = " << b
                              << ", target_f(b) = " << target_f(b) << ", c = " << c << "\n";

                    if (x <= a || x >= c) {
                        std::cout << "x выходит за границы, выход из цикла\n";
                        BREAK;  // ← Выход из цикла
                    } else {
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
                    }
                }
            }
        );

        std::cout << "Количество итераций: " << WhileCounter::getcount()->getinfo() << std::endl;

        return b;
    }
}

template<typename T>
auto porabola() {
    return &porabola_name::f<T>;
}
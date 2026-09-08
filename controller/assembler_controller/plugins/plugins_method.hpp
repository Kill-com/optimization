#pragma once

#include <functional>

#include "macros.hpp"

#include "../../../analisis/macros.hpp"

#include "../../../container/method_container.hpp"

template<typename T>
class SimpleSearch:public EPSContainer<T>,
    public TAUContainer<T>,
    public EContainer<T>{
protected:
    using EPSContainer<T>::EPS;
    using TAUContainer<T>::TAU;
    using EContainer<T>::E;
};

template<typename T>
class GOLD_SECH_:public SimpleSearch<T>{
private:
    using EPSContainer<T>::EPS;
    using TAUContainer<T>::TAU;
public:
    static T f(std::function<T(T)> target_f, T a, T b){
        T x1 = a + (1-TAU)*(b-a);  
        T x2 = a + TAU*(b-a);

        WHILE (b-a,[&](T){return b-a>EPS;},{
            x1 = a + (1-TAU)*(b-a);  
            x2 = a + TAU*(b-a);
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
        } );

        return(a+ b)/2;
    }
    REGISTER_FUNCTION
};

template<typename T>
class PORABOLA_:public SimpleSearch<T>{
private:
    using EPSContainer<T>::EPS;
public:
    static T f(std::function<T(T)> target_f, T a, T c) {
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
    REGISTER_FUNCTION
};
#pragma once

#include <functional>

#include "../macros/macros.hpp"

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

template<typename Output, typename Input=Output>
class IMethod{
private:
    template<typename... Args>
    static auto make_map(Args... args) {
        std::unordered_map<std::string, std::vector<Input>> map;
        int i = 1;
        ((map["x" + std::to_string(i++)]=
            std::vector<Input>{ static_cast<Input>(args) }), ...);
        return map;
    }
protected:
    using type_SelfWrite_foo =
    std::function<
    std::vector<Output>
    (const std::unordered_map<std::string, std::vector<Input>>&)>;

    using type_normal_foo= std::function<Output(Input)>;

    using type_var_foo= std::variant<type_SelfWrite_foo, type_normal_foo>;

    template<typename ...Args>
    static Output get_result_foo(const type_normal_foo& name, Args&& ...args){
        return name(std::forward<Args>(args)...);
    }

    template<typename A>
    static Output get_result_foo(const type_SelfWrite_foo& name, A&& a){
        return name(make_map<Output>(a))[0];
    }
    template<typename A, typename ...Args>
    static std::vector<Output> get_result_foo(const type_SelfWrite_foo& name,A&& a, Args&& ...args){
        return name(make_map<Output>(std::forward<A>(a), std::forward<Args>(args)...));
    }
};

template<typename T>
class GOLD_SECH_:public SimpleSearch<T>, protected IMethod<T>{
private:
    using EPSContainer<T>::EPS;
    using TAUContainer<T>::TAU;
    using Base = IMethod<T>;
    using typename Base::type_var_foo; 
    using Base::get_result_foo;
public:
    static T f(type_var_foo target_f, T a, T b){
        REPACK(target_f,
        T x1 = a + (1-TAU)*(b-a);  
        T x2 = a + TAU*(b-a);

        WHILE (b-a,[&](T){return b-a>EPS;},{
            x1 = a + (1-TAU)*(b-a);  
            x2 = a + TAU*(b-a);
            if (get_result_foo(target_f_, x1) < get_result_foo(target_f_, x2)){
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
    )
    }
    REGISTER_FUNCTION
};

template<typename T>
class PORABOLA_:public SimpleSearch<T>,protected IMethod<T>{
private:
    using EPSContainer<T>::EPS;
    using Base = IMethod<T>;
    using typename Base::type_var_foo;
    using Base::get_result_foo;
public:
    static T f(type_var_foo target_f, T a, T c) {
        REPACK(target_f,
        T b = (c + a) / 2;
        T x = b;          
        T b_old = b;   

        // Используем WHILE с поддержкой BREAK
        WHILE (0,
            [&](T) { 
                return (c - a) > EPS || std::abs(get_result_foo(target_f_,b) - get_result_foo(target_f_,b_old)) > EPS; 
            },
            {
                b_old = b;       
                T znam = (c - b) * get_result_foo(target_f_,a) + (a - c) * get_result_foo(target_f_,b) + (b - a) * get_result_foo(target_f_,c);
                
                if (std::abs(znam) < 1e-12) {
                    std::cout << "Вырожденная парабола, выход из цикла\n";
                    BREAK;  // ← Выход из цикла
                } else {
                    x = -0.5 * (((get_result_foo(target_f_,b) - get_result_foo(target_f_,a)) * (c - a) * (c - b) -
                                (a + b) * ((c - b) * get_result_foo(target_f_,a) + (a - c) * get_result_foo(target_f_,b) + (b - a) * get_result_foo(target_f_,c))) / znam);

                    if (x <= a || x >= c) {
                        std::cout << "x выходит за границы, выход из цикла\n";
                        BREAK;  // ← Выход из цикла
                    } else {
                        if (get_result_foo(target_f_ , x) < get_result_foo(target_f_, b)) {
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
    )
    }
    REGISTER_FUNCTION
};

#pragma once

#include <functional>
#include <cmath>
#include <algorithm>

#include "../includes.hpp"

#define WHILE_BRACKET(kef) \
    WHILE(0, [&](T f1, T f2){return f1 > f2;},{\
        h *= kef;\
        t0 = t1;\
        t1 = t2;\
        f0 = f1;\
        f1 = f2;\
        t2 = t1 + h;\
        f2 = get_result_foo(f_,t2);\
    });

#define REPACK_BRACKET(incre, def)\
    T h = ContainerBracket<T>::H;\
    T t0 = static_cast<T>(0);\
    T t1 = t0 + h;\
    REPACK(f,pair,{\
            T f0 = get_result_foo(f_,t0);\
            T f1 = get_result_foo(f_,t1);\
            if (f1 > f0) {\
                h = -h;\
                std::swap(t0, t1);\
                std::swap(f0, f1);\
            }\
            T t2 = t1 + h;\
            T f2 = get_result_foo(f_,t2);\
            WHILE_BRACKET(incre)\
            T t_sr = t1 + h/def;\
            T f_sr = get_result_foo(f_,t_sr);\
            if(f_sr < f1){\
                return {std::min(t1, t2), std::max(t1, t2)};\
            }else{\
                return {std::min(t0, t_sr), std::max(t0, t_sr)};\
            }\
        })
template<typename T>
class ContainerBracket:public HContainer<T>, public TAUContainer<T>{
protected: 
    using TAUContainer<T>::TAU;
    inline static T h = HContainer<T>::H;
};

#define CONTAINER_BRACKET\
    using T=scalar_of_t<U>;\
    using pair=std::pair<T,T>;\
    using ContainerBracket<T>::TAU;
template<typename U>
class BRACKETGEOM_: public ContainerBracket<scalar_of_t<U>>,public IMethod<scalar_of_t<U>,scalar_of_t<U>>{
    CONTAINER_BRACKET
    USING_ALL(T,T)
public:
    static pair f(type_var_foo f){
        REPACK_BRACKET(2,2)
    }
    REGISTER_FUNCTION
};

template<typename U>
class BRACKETTAU_: public ContainerBracket<scalar_of_t<U>>,public IMethod<scalar_of_t<U>,scalar_of_t<U>> {
private:
    CONTAINER_BRACKET
    USING_ALL(T,T)
public:
    static pair f(type_var_foo f) {
       REPACK_BRACKET(TAU, TAU)
    }
    REGISTER_FUNCTION
};

template<typename U>
class BRACKETTAU2_: public ContainerBracket<scalar_of_t<U>>, public IMethod<scalar_of_t<U>,scalar_of_t<U>> {
private:
    CONTAINER_BRACKET
    USING_ALL(T,T)
public:
    static pair f(type_var_foo f) {
        REPACK_BRACKET(TAU*TAU, TAU)
    }
    REGISTER_FUNCTION
};

template<typename U>
class BRACKETTAU3_: public ContainerBracket<scalar_of_t<U>>,public IMethod<scalar_of_t<U>,scalar_of_t<U>> {
private:
    CONTAINER_BRACKET
    USING_ALL(T,T)
public:
    static pair f(type_var_foo f) {
        REPACK_BRACKET(TAU*TAU*TAU,TAU)
    }
    REGISTER_FUNCTION
};

template<typename U>
class BRACKETCONSTANT_: public ContainerBracket<scalar_of_t<U>>,public IMethod<scalar_of_t<U>,scalar_of_t<U>> {
private:
    CONTAINER_BRACKET
    USING_ALL(T,T)
public:
    static pair f(type_var_foo f) {
        T h = ContainerBracket<T>::H;
        T t0 = static_cast<T>(0);
        T t1 = t0 + h;
        REPACK(f,pair,{
            T f0 = get_result_foo(f_,t0);
            T f1 = get_result_foo(f_,t1);
    
            if (f1 > f0) {
                h = -h;
                std::swap(t0, t1);
                std::swap(f0, f1);
            }
    
            T t2 = t1 + h;
            T f2 = get_result_foo(f_,t2);
    
            WHILE(0, [&](T f1, T f2){ return f1 >= f2; }, {
                t0 = t1;
                f0 = f1;
                t1 = t2;
                f1 = f2;
                t2 += h;
                f2 = get_result_foo(f_,t2);
            });
    
            return {std::min(t0, t2), std::max(t0, t2)};
        })
    }
    REGISTER_FUNCTION
};
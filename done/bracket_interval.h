#pragma once

#include <functional>
#include <cmath>
#include <algorithm>

template<typename T, typename Func>
std::pair<T, T> bracket_geom(Func f){
    T h = static_cast<T>(0.01);
    T t0 = static_cast<T>(0);
    T t1 = t0 + h;
    T f0 = f(t0);
    T f1 = f(t1);

    if (f1 > f0) {
        h = -h;
        std::swap(t0, t1);
        std::swap(f0, f1);
    }

    T t2 = t1 + h;
    T f2 = f(t2);

    while (f1 > f2) {
        h *= 2;
        t0 = t1;
        t1 = t2;
        f0 = f1;
        f1 = f2;
        t2 = t1 + h;
        f2 = f(t2);
    }

    T t_sr = t1 + h/2;
    T f_sr = f(t_sr);

    if(f_sr < f1){
        return {std::min(t1, t2), std::max(t1, t2)};
    }else{
        return {std::min(t0, t_sr), std::max(t0, t_sr)};
    }
}


template<typename T, typename Func>
std::pair<T, T> bracket_phi(Func f){
    T phi = static_cast<T>((1+sqrt(5))/2);
    T h = static_cast<T>(0.01);
    T t0 = static_cast<T>(0);
    T t1 = t0 + h;
    T f0 = f(t0);
    T f1 = f(t1);

    if (f1 > f0) {
        h = -h;
        std::swap(t0, t1);
        std::swap(f0, f1);
    }

    T t2 = t1 + h;
    T f2 = f(t2);

    while (f1 > f2) {
        h *= phi;
        t0 = t1;
        t1 = t2;
        f0 = f1;
        f1 = f2;
        t2 = t1 + h;
        f2 = f(t2);
    }

    T t_sr = t1 + h/phi;
    T f_sr = f(t_sr);

    if(f_sr < f1){
        return {std::min(t1, t2), std::max(t1, t2)};
    }else{
        return {std::min(t0, t_sr), std::max(t0, t_sr)};
    }
    
}

template<typename T, typename Func>
std::pair<T, T> bracket_phi2(Func f){
    T phi = static_cast<T>((1+sqrt(5))/2);
    T h = static_cast<T>(0.01);
    T t0 = static_cast<T>(0);
    T t1 = t0 + h;
    T f0 = f(t0);
    T f1 = f(t1);

    if (f1 > f0) {
        h = -h;
        std::swap(t0, t1);
        std::swap(f0, f1);
    }

    T t2 = t1 + h;
    T f2 = f(t2);

    while (f1 > f2) {
        h *= phi*phi;
        t0 = t1;
        t1 = t2;
        f0 = f1;
        f1 = f2;
        t2 = t1 + h;
        f2 = f(t2);
    }

    T t_sr = t1 + h/phi;
    T f_sr = f(t_sr);

    if(f_sr < f1){
        return {std::min(t1, t2), std::max(t1, t2)};
    }else{
        return {std::min(t0, t_sr), std::max(t0, t_sr)};
    }
    
}


template<typename T, typename Func>
std::pair<T, T> bracket_phi3(Func f){
    T phi = static_cast<T>((1+sqrt(5))/2);
    T h = static_cast<T>(0.01);
    T t0 = static_cast<T>(0);
    T t1 = t0 + h;
    T f0 = f(t0);
    T f1 = f(t1);

    if (f1 > f0) {
        h = -h;
        std::swap(t0, t1);
        std::swap(f0, f1);
    }

    T t2 = t1 + h;
    T f2 = f(t2);

    while (f1 > f2) {
        h *= phi*phi*phi;
        t0 = t1;
        t1 = t2;
        f0 = f1;
        f1 = f2;
        t2 = t1 + h;
        f2 = f(t2);
    }

    T t_sr = t1 + h/phi;
    T f_sr = f(t_sr);

    if(f_sr < f1){
        return {std::min(t1, t2), std::max(t1, t2)};
    }else{
        return {std::min(t0, t_sr), std::max(t0, t_sr)};
    }
    
}

template<typename T, typename Func>
std::pair<T, T> bracket_constant(Func f) {
    T h = static_cast<T>(0.01);
    T t0 = static_cast<T>(0);
    T t1 = t0 + h;
    T f0 = f(t0);
    T f1 = f(t1);

    if (f1 > f0) {
        h = -h;
        std::swap(t0, t1);
        std::swap(f0, f1);
    }

    T t2 = t1 + h;
    T f2 = f(t2);

    while(f1 >= f2){
        t0 = t1;
        f0 = f1;
        t1 = t2;
        f1 = f2;
        t2 += h;
        f2 = f(t2);
    }

    return {std::min(t0, t2), std::max(t0, t2)};
}
// method_container.hpp
#pragma once
#define _USE_MATH_DEFINES 
#include <cmath>
#include "container.hpp"

template<typename T>
class EPSContainer {
protected:
    static inline T EPS = static_cast<T>(1e-6);
public:
    static void setEps(T eps_) {
        EPS = eps_;
    }
};

template<typename T>
class TAUContainer {
protected:
    static inline const T TAU = static_cast<T>((std::sqrt(5.0) - 1.0) / 2.0);
};

template<typename T>
class EContainer {
protected:
    static inline const T E = static_cast<T>(M_E);
};
#pragma once

#include <functional>
#include <cmath>
#include <iostream>

template<typename T, typename Func>
T golden_section(Func f, std::pair<T, T> para) {
    const double TAU = 0.6180339887498949;
    const double EPS = 1e-6;
    T a = para.first;
    T b = para.second;
    T x1 = a + (1 - TAU) * (b - a);
    T x2 = a + TAU * (b - a);
    T fx1 = f(x1);
    T fx2 = f(x2);


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
    }
    return (a + b) / 2;
}


template<typename T, typename Func>
T parabola_method(Func f, std::pair<T, T> para) {
    const T EPS = 1e-6;
    T a  = para.first;
    T c = para.second;
    T b = (c + a) / 2;
    T x = b;
    T b_old = b;

    T fa = f(a);
    T fb = f(b);
    T fc = f(c);
    T fb_old = fb;

    do {
        b_old = b;
        fb_old = fb;

        T ca = c - a;
        T cb = c - b;
        T ab = a + b;

        T znam = cb * fa + (a - c) * fb + (b - a) * fc;
        if (std::abs(znam) < 1e-12) {
            break;          // вырожденная парабола
        }
        x = -0.5f * (((fb - fa) * ca * cb - ab * znam) / znam);
        T fx = f(x);

        if (x <= a || x >= c) {
            break;
        }

        if (fx < fb) {
            if (x < b) {
                c = b;
                fc = fb;
                b = x;
                fb = fx;
            } else {
                a = b;
                fa = fb;
                b = x;
                fb = fx;
            }
        } else {
            if (x < b) {
                a = x;
                fa = fx;
            } else {
                c = x;
                fc = fx;
            }
        }

    } while ((c - a) > EPS || std::abs(fb - fb_old) > EPS);

    return b;
}
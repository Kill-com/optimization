#include <functional>
#include <cmath>
#include <iostream>

template<typename T, typename Func>
T parabola_method(Func f, T a, T c) {
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

        std::cout << "x = " << x << ", f(x) = " << fx
                  << ", a = " << a << ", b = " << b
                  << ", f(b) = " << fb << ", c = " << c << "\n";

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

#pragma once

#include "includes.hpp"

template<typename U>
class GRAD_SPYSK_: public SimpleSearch<scalar_of_t<U>>, protected IMethod<scalar_of_t<U>>{
private:
    using T = scalar_of_t<U>;
    USING_ALL(T)

    using Point = std::vector<T>;

    static T norm(const Point& x) {
        T sum = 0.0;
        for (T v : x) {
            sum += v * v;
        }
        return std::sqrt(sum);
    }

    static T rosenbrock(const Point& x) {
        return std::pow(1 - x[0], 2)
             + 100 * std::pow(x[1] - x[0] * x[0], 2);
    }

    static Point rosenbrock_grad(const Point& x) {
        int n = static_cast<int>(x.size());
        Point g(n, 0.0);
        g[0] = -2 * (1 - x[0]) - 400 * x[0] * (x[1] - x[0] * x[0]);
        g[1] = 200 * (x[1] - x[0] * x[0]);
        return g;
    }

    static T line_search(
        std::function<T(const Point&)> f,
        std::function<Point(const Point&)> grad,
        const Point& x,
        const Point& d,
        T alpha = 1.0,
        T beta = 1e-4,
        T rho = 0.5
    ) {
        int n = static_cast<int>(x.size());
        T fx = f(x);
        Point g = grad(x);
        T gtd = 0.0;
        for (int i = 0; i < n; i++) {
            gtd += g[i] * d[i];
        }
        Point x_new(n, 0.0);
        while (true) {
            for (int i = 0; i < n; i++) {
                x_new[i] = x[i] + alpha * d[i];
            }
            if (f(x_new) <= fx + beta * alpha * gtd) {
                break;
            }
            alpha *= rho;
            if (alpha < 1e-12) {
                break;
            }
        }
        return alpha;
    }

    static Point gradient_descent(
        std::function<T(const Point&)> f,
        std::function<Point(const Point&)> grad,
        Point start,
        T tol = 1e-8,
        int max_iter = 1000
    ) {
        Point x = start;
        int n = static_cast<int>(x.size());

        for (int iter = 0; iter < max_iter; iter++) {
            Point g = grad(x);
            if (norm(g) < tol) {
                break;
            }
            Point d(n, 0.0);
            for (int i = 0; i < n; i++) {
                d[i] = -g[i];
            }
            T alpha = line_search(f, grad, x, d);
            for (int i = 0; i < n; i++) {
                x[i] += alpha * d[i];
            }

            LOG_METHOD(f(x));
        }
        return x;
    }

public:
    static T f(type_var_foo target_f, T a, T b) {
        (void)target_f;
        REPACK(target_f, T,
            (void)target_f_;
            Point start = {a, b};
            Point result = gradient_descent(
                rosenbrock,
                rosenbrock_grad,
                start,
                1e-8,
                1000
            );
            return rosenbrock(result);
        );
    }
    REGISTER_FUNCTION
};

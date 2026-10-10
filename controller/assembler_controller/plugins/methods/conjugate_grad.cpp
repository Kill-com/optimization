#pragma once

#include "includes.hpp"

template<typename U>
class CONJUGATE_GRAD_: public SimpleSearch<scalar_of_t<U>>, protected IMethod<scalar_of_t<U>>{
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

    static T dot(const Point& a, const Point& b) {
        T sum = 0.0;
        for (size_t i = 0; i < a.size(); i++) {
            sum += a[i] * b[i];
        }
        return sum;
    }

    static T rosenbrock(const Point& x) {
        return std::pow(1 - x[0], 2)
             + 100 * std::pow(x[1] - x[0] * x[0], 2);
    }

    static Point rosenbrock_grad(const Point& x) {
        Point g(2);
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
        T fx = f(x);
        Point g = grad(x);
        T gtd = dot(g, d);
        Point x_new(x.size());
        while (true) {
            for (size_t i = 0; i < x.size(); i++) {
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

    static Point conjugate_gradient(
        std::function<T(const Point&)> f,
        std::function<Point(const Point&)> grad,
        Point start,
        T tol = 1e-8,
        int max_iter = 1000
    ) {
        Point x = start;
        Point g = grad(x);
        Point d(x.size());
        for (size_t i = 0; i < x.size(); i++) {
            d[i] = -g[i];
        }

        for (int iter = 0; iter < max_iter; iter++) {
            if (norm(g) < tol) {
                break;
            }

            T alpha = line_search(f, grad, x, d);
            Point x_new = x;
            for (size_t i = 0; i < x.size(); i++) {
                x_new[i] += alpha * d[i];
            }

            Point g_new = grad(x_new);
            Point diff(x.size());
            for (size_t i = 0; i < x.size(); i++) {
                diff[i] = g_new[i] - g[i];
            }

            T beta = dot(g_new, diff) / dot(g, g);
            if (beta < 0) beta = 0;

            for (size_t i = 0; i < x.size(); i++) {
                d[i] = -g_new[i] + beta * d[i];
            }

            x = x_new;
            g = g_new;

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
            Point result = conjugate_gradient(
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

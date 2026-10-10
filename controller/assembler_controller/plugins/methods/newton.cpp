#pragma once

#include "includes.hpp"

template<typename U>
class NEWTON_: public SimpleSearch<scalar_of_t<U>>, protected IMethod<scalar_of_t<U>>{
private:
    using T = scalar_of_t<U>;
    USING_ALL(T)

    using Point = std::vector<T>;
    using Matrix = std::vector<std::vector<T>>;

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
        Point g(2);
        g[0] = -2 * (1 - x[0]) - 400 * x[0] * (x[1] - x[0] * x[0]);
        g[1] = 200 * (x[1] - x[0] * x[0]);
        return g;
    }

    static Matrix rosenbrock_hessian(const Point& x) {
        Matrix H(2, std::vector<T>(2));
        H[0][0] = 2 - 400 * x[1] + 1200 * x[0] * x[0];
        H[0][1] = -400 * x[0];
        H[1][0] = -400 * x[0];
        H[1][1] = 200;
        return H;
    }

    static Point solve_system(const Matrix& H, const Point& g) {
        Point d(2);
        T a = H[0][0];
        T b = H[0][1];
        T c = H[1][0];
        T e = H[1][1];
        T det = a * e - b * c;
        d[0] = (-g[0] * e + b * g[1]) / det;
        d[1] = (-a * g[1] + c * g[0]) / det;
        return d;
    }

    static Point newton_method(
        std::function<T(const Point&)> f,
        std::function<Point(const Point&)> grad,
        std::function<Matrix(const Point&)> hessian,
        Point start,
        T tol = 1e-8,
        int max_iter = 1000
    ) {
        Point x = start;
        for (int iter = 0; iter < max_iter; iter++) {
            Point g = grad(x);
            if (norm(g) < tol) {
                break;
            }
            Matrix H = hessian(x);
            Point d = solve_system(H, g);
            for (size_t i = 0; i < x.size(); i++) {
                x[i] += d[i];
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
            Point result = newton_method(
                rosenbrock,
                rosenbrock_grad,
                rosenbrock_hessian,
                start,
                1e-8,
                1000
            );
            return rosenbrock(result);
        );
    }
    REGISTER_FUNCTION
};

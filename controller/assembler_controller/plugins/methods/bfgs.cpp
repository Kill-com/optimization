#pragma once

#include "includes.hpp"

template<typename U>
class BFGS_: public SimpleSearch<scalar_of_t<U>>, protected IMethod<scalar_of_t<U>>{
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
        return std::pow(1 - x[0], 2) + 100 * std::pow(x[1] - x[0] * x[0], 2);
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

        while (alpha > 1e-12) {
            for (size_t i = 0; i < x.size(); i++) {
                x_new[i] = x[i] + alpha * d[i];
            }
            if (f(x_new) <= fx + beta * alpha * gtd) {
                break;
            }
            alpha *= rho;
        }
        return alpha;
    }

    static Point matrix_vector(
        const std::vector<std::vector<T>>& H,
        const Point& x
    ) {
        Point result(x.size(), 0.0);
        for (size_t i = 0; i < x.size(); i++) {
            for (size_t j = 0; j < x.size(); j++) {
                result[i] += H[i][j] * x[j];
            }
        }
        return result;
    }

    static std::vector<std::vector<T>> matrix_mult(
        const std::vector<std::vector<T>>& A,
        const std::vector<std::vector<T>>& B
    ) {
        size_t n = A.size();
        std::vector<std::vector<T>> C(n, std::vector<T>(n, 0.0));
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                for (size_t k = 0; k < n; k++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
        return C;
    }

    static Point bfgs_method(
        std::function<T(const Point&)> f,
        std::function<Point(const Point&)> grad,
        Point start,
        T tol = 1e-8,
        int max_iter = 1000
    ) {
        size_t n = start.size();
        Point x = start;
        std::vector<std::vector<T>> H(n, std::vector<T>(n, 0.0));
        for (size_t i = 0; i < n; i++) {
            H[i][i] = 1.0;
        }
        Point g = grad(x);

        for (int iter = 0; iter < max_iter; iter++) {
            if (norm(g) < tol) {
                break;
            }

            Point Hg = matrix_vector(H, g);
            Point d(n);
            for (size_t i = 0; i < n; i++) {
                d[i] = -Hg[i];
            }

            T alpha = line_search(f, grad, x, d);

            Point x_new = x;
            for (size_t i = 0; i < n; i++) {
                x_new[i] += alpha * d[i];
            }

            Point g_new = grad(x_new);
            Point s(n);
            Point y(n);
            for (size_t i = 0; i < n; i++) {
                s[i] = x_new[i] - x[i];
                y[i] = g_new[i] - g[i];
            }

            T ys = dot(y, s);
            if (ys > 1e-12) {
                T rho = 1.0 / ys;

                std::vector<std::vector<T>> I(n, std::vector<T>(n, 0.0));
                for (size_t i = 0; i < n; i++) {
                    I[i][i] = 1.0;
                }

                std::vector<std::vector<T>> A(n, std::vector<T>(n));
                std::vector<std::vector<T>> B(n, std::vector<T>(n));
                for (size_t i = 0; i < n; i++) {
                    for (size_t j = 0; j < n; j++) {
                        A[i][j] = I[i][j] - rho * s[i] * y[j];
                        B[i][j] = I[i][j] - rho * y[i] * s[j];
                    }
                }

                std::vector<std::vector<T>> temp = matrix_mult(A, H);
                std::vector<std::vector<T>> H_new = matrix_mult(temp, B);
                for (size_t i = 0; i < n; i++) {
                    for (size_t j = 0; j < n; j++) {
                        H_new[i][j] += rho * s[i] * s[j];
                    }
                }
                H = H_new;
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
            Point result = bfgs_method(
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

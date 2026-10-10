#pragma once

#include "includes.hpp"

template<typename U>
class NELDERMEAD_: public SimpleSearch<scalar_of_t<U>>, protected IMethod<scalar_of_t<U>>{
private:
    using T = scalar_of_t<U>;
    USING_ALL(T)

    using Point = std::vector<T>;

    class NelderMead {
    private:
        T alpha;
        T gamma;
        T rho;
        T sigma;

    public:
        NelderMead(
            T alpha = 1.0,
            T gamma = 2.0,
            T rho   = 0.5,
            T sigma = 0.5
        ) : alpha(alpha),
            gamma(gamma),
            rho(rho),
            sigma(sigma)
        {}

        std::pair<Point, T> minimize(
            std::function<T(const Point&)> f,
            Point start,
            T step = 1.0,
            T tol  = 1e-8,
            int maxIter = 1000
        ) {
            int n = static_cast<int>(start.size());
            std::vector<Point> simplex(n + 1, start);
            for (int i = 0; i < n; i++) {
                simplex[i + 1][i] += step;
            }

            for (int iter = 0; iter < maxIter; iter++) {
                std::sort(simplex.begin(), simplex.end(),
                    [&](const Point& a, const Point& b) {
                        return f(a) < f(b);
                    });

                std::vector<T> values(n + 1);
                for (int i = 0; i <= n; i++) {
                    values[i] = f(simplex[i]);
                }

                LOG_METHOD(values[0]);

                T diff = 0;
                for (int i = 1; i <= n; i++) {
                    diff = std::max(diff, std::fabs(values[i] - values[0]));
                }
                if (diff < tol)
                    break;

                Point best = simplex[0];
                Point worst = simplex[n];
                Point centroid(n, 0.0);
                for (int i = 0; i < n; i++) {
                    for (int j = 0; j < n; j++) {
                        centroid[j] += simplex[i][j];
                    }
                }
                for (T& x : centroid) {
                    x /= n;
                }

                Point xr(n);
                for (int i = 0; i < n; i++) {
                    xr[i] = centroid[i] + alpha * (centroid[i] - worst[i]);
                }

                T fr = f(xr);

                if (fr < values[0]) {
                    Point xe(n);
                    for (int i = 0; i < n; i++) {
                        xe[i] = centroid[i] + gamma * (xr[i] - centroid[i]);
                    }
                    T fe = f(xe);
                    if (fe < fr) {
                        simplex[n] = xe;
                    }
                    else {
                        simplex[n] = xr;
                    }
                }
                else if (fr < values[n - 1]) {
                    simplex[n] = xr;
                }
                else {
                    Point xc(n);
                    for (int i = 0; i < n; i++) {
                        xc[i] = centroid[i] + rho * (worst[i] - centroid[i]);
                    }
                    T fc = f(xc);
                    if (fc < values[n]) {
                        simplex[n] = xc;
                    }
                    else {
                        for (int i = 1; i <= n; i++) {
                            for (int j = 0; j < n; j++) {
                                simplex[i][j] = best[j] + sigma * (simplex[i][j] - best[j]);
                            }
                        }
                    }
                }
            }

            std::sort(simplex.begin(), simplex.end(),
                [&](const Point& a, const Point& b) {
                    return f(a) < f(b);
                });

            return {simplex[0], f(simplex[0])};
        }
    };

    static T parabola_target(const Point& x) {
        return std::pow(x[0] - 2, 2) + std::pow(x[1] - 3, 2);
    }

public:
    static T f(type_var_foo target_f, T a, T b) {
        (void)target_f;
        REPACK(target_f, T,
            (void)target_f_;
            NelderMead nm;
            Point start = {a, b};
            auto result = nm.minimize(parabola_target, start, 1.0);
            return result.second;
        );
    }
    REGISTER_FUNCTION
};

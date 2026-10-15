#pragma once
#include "includes.hpp"

template<typename U>
class NELDERMEAD_: public SimpleSearch<scalar_of_t<U>>,
                   protected IMethod<scalar_of_t<U>, U>{
private:
    using T = scalar_of_t<U>;
    USING_ALL(T, U)
    using Point = std::vector<T>;

public:
    static T f(type_var_foo target_f, U arg){
        REPACK(target_f, T,
            auto eval = [&](const Point& p) -> T {
                return get_result_foo(target_f_, p);
            };

            T alpha = 1, gamma = 2, rho = 0.5, sigma = 0.5;
            T step = 1, tol = static_cast<T>(1e-8);
            const int n = 2;

            Point start = {static_cast<T>(arg[0]), static_cast<T>(arg[1])};
            std::vector<Point> S(n + 1, start);
            S[1][0] += step;
            S[2][1] += step;

            FOR(0, 1000, {
                std::sort(S.begin(), S.end(),
                    [&](const Point& a, const Point& b){ return eval(a) < eval(b); });

                std::vector<T> v(n + 1);
                for (int i = 0; i <= n; ++i) v[i] = eval(S[i]);
                LOG_METHOD(v[0]);

                T diff = 0;
                for (int i = 1; i <= n; ++i)
                    diff = std::max(diff, std::fabs(v[i] - v[0]));
                if (diff < tol) BREAK;

                Point best = S[0], worst = S[n];
                Point c(n, 0);
                for (int i = 0; i < n; ++i)
                    for (int j = 0; j < n; ++j) c[j] += S[i][j];
                for (int j = 0; j < n; ++j) c[j] /= n;

                Point xr(n);
                for (int i = 0; i < n; ++i) xr[i] = c[i] + alpha * (c[i] - worst[i]);
                T fr = eval(xr);

                if (fr < v[0]) {
                    Point xe(n);
                    for (int i = 0; i < n; ++i) xe[i] = c[i] + gamma * (xr[i] - c[i]);
                    if (eval(xe) < fr) S[n] = xe; else S[n] = xr;
                } else if (fr < v[n - 1]) {
                    S[n] = xr;
                } else {
                    Point xc(n);
                    for (int i = 0; i < n; ++i) xc[i] = c[i] + rho * (worst[i] - c[i]);
                    if (eval(xc) < v[n]) S[n] = xc;
                    else {
                        for (int i = 1; i <= n; ++i)
                            for (int j = 0; j < n; ++j)
                                S[i][j] = best[j] + sigma * (S[i][j] - best[j]);
                    }
                }
            });

            std::sort(S.begin(), S.end(),
                [&](const Point& a, const Point& b){ return eval(a) < eval(b); });
            return eval(S[0]);
        );
    }
    REGISTER_FUNCTION
};

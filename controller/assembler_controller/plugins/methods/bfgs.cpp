#pragma once
#include "includes.hpp"

template<typename U>
class BFGS_: public SimpleSearch<scalar_of_t<U>>,
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
            auto grad_at = [&](const Point& p) -> Point {
                const T h = static_cast<T>(1e-6);
                T f0 = eval(p);
                Point g(2);
                Point pp = p; pp[0] += h; g[0] = (eval(pp) - f0) / h;
                pp = p;       pp[1] += h; g[1] = (eval(pp) - f0) / h;
                return g;
            };
            auto dot = [](const Point& a, const Point& b){
                T s = 0; for(size_t i=0;i<a.size();++i) s += a[i]*b[i]; return s;
            };
            auto nrm = [&](const Point& v){ return std::sqrt(dot(v,v)); };

            Point x = {static_cast<T>(arg[0]), static_cast<T>(arg[1])};
            Point g = grad_at(x);
            std::vector<std::vector<T>> H = {{1,0},{0,1}};

            FOR(0, 1000, {
                if (nrm(g) < static_cast<T>(1e-8)) BREAK;

                Point Hg(2);
                for (int i = 0; i < 2; ++i)
                    for (int j = 0; j < 2; ++j) Hg[i] += H[i][j] * g[j];
                Point d = {-Hg[0], -Hg[1]};

                T fx = eval(x);
                T gtd = g[0]*d[0] + g[1]*d[1];
                T alpha = 1;
                Point xn = {x[0] + alpha*d[0], x[1] + alpha*d[1]};
                WHILE(0, [&](int){
                    return alpha > static_cast<T>(1e-12)
                        && eval(xn) > fx + static_cast<T>(1e-4)*alpha*gtd;
                }, {
                    alpha *= 0.5;
                    xn = {x[0] + alpha*d[0], x[1] + alpha*d[1]};
                });

                Point gn = grad_at(xn);
                Point s  = {xn[0]-x[0],  xn[1]-x[1]};
                Point y  = {gn[0]-g[0],  gn[1]-g[1]};
                T ys = dot(y, s);
                if (ys > 1e-12) {
                    T rho = 1 / ys;
                    std::vector<std::vector<T>> A(2, std::vector<T>(2));
                    std::vector<std::vector<T>> B(2, std::vector<T>(2));
                    for (int i=0;i<2;++i) for (int j=0;j<2;++j){
                        T ij = (i==j) ? 1 : 0;
                        A[i][j] = ij - rho*s[i]*y[j];
                        B[i][j] = ij - rho*y[i]*s[j];
                    }
                    std::vector<std::vector<T>> tmp(2, std::vector<T>(2));
                    std::vector<std::vector<T>> Hn (2, std::vector<T>(2));
                    for (int i=0;i<2;++i) for (int j=0;j<2;++j){
                        for (int k=0;k<2;++k) tmp[i][j] += A[i][k] * H[k][j];
                        for (int k=0;k<2;++k) Hn[i][j]  += tmp[i][k] * B[k][j];
                        Hn[i][j] += rho * s[i] * s[j];
                    }
                    H = Hn;
                }
                x = xn; g = gn;
                LOG_METHOD(eval(x));
            });
            return eval(x);
        );
    }
    REGISTER_FUNCTION
};

#pragma once
#include "includes.hpp"

template<typename U>
class NEWTON_: public SimpleSearch<scalar_of_t<U>>,
               protected IMethod<scalar_of_t<U>, U>{
private:
    using T = scalar_of_t<U>;
    USING_ALL(T, U)
    using Point = std::vector<T>;
    using Mat   = std::vector<std::vector<T>>;

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
            auto hess_at = [&](const Point& p) -> Mat {
                const T h = static_cast<T>(1e-4);
                T f0 = eval(p);
                Point px = {p[0]+h, p[1]}, mx = {p[0]-h, p[1]};
                Point py = {p[0], p[1]+h}, my = {p[0], p[1]-h};
                T fxx = (eval(px) - 2*f0 + eval(mx)) / (h*h);
                T fyy = (eval(py) - 2*f0 + eval(my)) / (h*h);
                T fp = eval(Point{p[0]+h, p[1]+h});
                T fm = eval(Point{p[0]+h, p[1]-h});
                T gp = eval(Point{p[0]-h, p[1]+h});
                T gm = eval(Point{p[0]-h, p[1]-h});
                T fxy = (fp - fm - gp + gm) / (4*h*h);
                return {{fxx, fxy}, {fxy, fyy}};
            };
            auto nrm = [](const Point& v){ return std::sqrt(v[0]*v[0] + v[1]*v[1]); };

            Point x = {static_cast<T>(arg[0]), static_cast<T>(arg[1])};

            FOR(0, 1000, {
                Point g = grad_at(x);
                if (nrm(g) < static_cast<T>(1e-8)) BREAK;
                Mat H = hess_at(x);
                T det = H[0][0]*H[1][1] - H[0][1]*H[1][0];
                if (std::fabs(det) < 1e-12) BREAK;
                Point d;
                d.push_back((-g[0]*H[1][1] + H[0][1]*g[1]) / det);
                d.push_back((-H[0][0]*g[1] + H[1][0]*g[0]) / det);
                x[0] += d[0];
                x[1] += d[1];
                LOG_METHOD(eval(x));
            });
            return eval(x);
        );
    }
    REGISTER_FUNCTION
};

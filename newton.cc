#include <functional>
#include <cmath>
#include <iostream>

const double EPS = 1e-6;
const double H = 1e-4;

double f(double x)
{
    return (x-2)*(x-2) + std::sin(x);
}

double derivative1(std::function<double(double)> f, double x)
{
    return (f(x + H) - f(x - H)) / (2.0f * H);
}

double derivative2(std::function<double(double)> f, double x)
{
    return (f(x + H) - 2.0f * f(x) + f(x - H)) / (H * H);
}

double Newton(std::function<double(double)> f, double x)
{
    do
    {
        double df = derivative1(f, x);
        double ddf = derivative2(f, x);

        std::cout << "x = " << x  << ", f'(x) = " << df << ", f''(x) = " << ddf << std::endl;

        if (std::abs(df) < EPS)
        {
            break;
        }

        if (std::abs(ddf) < EPS)
        {
            break;
        }

        double x_new = x - df / ddf;

        if (std::abs(x_new - x) < EPS)
        {
            x = x_new;
            break;
        }

        x = x_new;
    }
    while (true);

    return x;
}

int main()
{
    double res = Newton(f, 0.0f);
    std::cout << "Результат: x = " << res << ", y = " << f(res) << std::endl;
    return 0;
}

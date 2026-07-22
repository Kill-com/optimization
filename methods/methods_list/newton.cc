const double EPS = 1e-6;
const double H = 1e-4;
const int max_iter = 100;

double derivative1(std::function<double(double)> f, double x)
{
    return (f(x + H) - f(x - H)) / (2.0 * H);
}

double derivative2(std::function<double(double)> f, double x)
{
    return (f(x + H) - 2.0 * f(x) + f(x - H)) / (H * H);
}

double Newton(std::function<double(double)> f, double x)
{
    for (int i = 0; i < max_iter; i++)
    {
        double df = derivative1(f, x);
        double ddf = derivative2(f, x);

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

    return x;
}

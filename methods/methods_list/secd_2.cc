template<typename T>
T Element(const std::function<T(const std::vector<T>&)>& f,
          const std::vector<T>& x,
          int i, int j,
          T H)
{
    if (i == j)
    {
        std::vector<T> x_i = x;
        std::vector<T> x_d = x;
        x_i[i] += H;
        x_d[i] -= H;
        return (f(x_i) - static_cast<T>(2) * f(x) + f(x_d)) / (H * H);
    }
    else
    {
        std::vector<T> x_ii = x, x_id = x, x_di = x, x_dd = x;
        x_ii[i] += H; x_ii[j] += H;
        x_id[i] += H; x_id[j] -= H;
        x_di[i] -= H; x_di[j] += H;
        x_dd[i] -= H; x_dd[j] -= H;
        return (f(x_ii) - f(x_id) - f(x_di) + f(x_dd)) / (static_cast<T>(4) * H * H);
    }
}

template<typename T>
std::vector<std::vector<T>> createHessian(
    const std::function<T(const std::vector<T>&)>& f,
    const std::vector<T>& x,
    T H)
{
    int n = x.size();
    std::vector<std::vector<T>> Hessian(n, std::vector<T>(n, static_cast<T>(0)));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            Hessian[i][j] = Element<T>(f, x, i, j, H);
    return Hessian;
}

template<typename T>
T dirDerivative(const std::function<T(const std::vector<T>&)>& f,
                const std::vector<T>& x,
                const std::vector<T>& v,
                T H)
{
    auto Hes = createHessian<T>(f, x, H);
    int n = x.size();
    T res = static_cast<T>(0);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            res += v[i] * Hes[i][j] * v[j];
    return res;
}
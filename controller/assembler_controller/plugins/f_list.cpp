
#include <cmath>


template<typename T>
T target_f(T x) {
    return std::pow((x-2), 2) + std::sin(x);
}

template<typename T>
auto f_list(){
    return &target_f<T>;
}

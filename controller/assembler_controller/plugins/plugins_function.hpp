#pragma once

#include "../../../container/method_container.hpp"
template<typename T>
class F_LIST_{
public:
    static T f(T x) {
        return std::pow((x-2), 2) + std::sin(x);
    }
    REGISTER_FUNCTION
};

template<typename T>
class INTRESTIN_F_{
    using U=scalar_of_t<T>;
public:
static U f(T arg){
    //return (4-2.1*arg[0]*arg[0] + 3*arg[0]*arg[0]*arg[0]*arg[0])*arg[0]*arg[0] + arg[1]*arg[0] + (-4+4*arg[1]*arg[1])*arg[1]*arg[1];
    return 4*exp(-arg[1]*arg[1]/4) * sin(2*arg[0]- 1.414);
}
REGISTER_FUNCTION
};

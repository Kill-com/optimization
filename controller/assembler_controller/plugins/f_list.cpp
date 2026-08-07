
#include <cmath>


static float target_f(float x) {
    return std::pow((x-2), 2) + std::sin(x);
}


float(*f_list())(float){
    return &target_f;
}

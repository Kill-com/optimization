#include <cmath>

extern "C" {
float target_f(float x) {
    return std::pow((x-2), 2) + std::sin(x);
}
}

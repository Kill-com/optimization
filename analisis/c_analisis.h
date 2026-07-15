#pragma once
#include <chrono>

#include "../exect/execution.hpp"

//анализ методов при функциях

//класс таймера
class CycleCounter {
public:
    static uint64_t rdtsc();
};



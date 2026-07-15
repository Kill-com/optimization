#pragma once
#include <chrono>

//анализ методов при функциях

//класс таймера

class CycleCounter {
public:
    static uint64_t rdtsc();
};

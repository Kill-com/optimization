#pragma once
#include <chrono>

#include "../exect/execution.h"

//анализ методов при функциях

//класс таймера
class CycleCounter {
public:
    static uint64_t rdtsc();
};

//шаблон для анализа
class ANALIS_EXECT : protected METHOD_L{
    private:
    METHOD_L* ex;
    public:
    ANALIS_EXECT(METHOD_L* m): ex(m){};
    void exect(std::string_view method, std::string_view function);
};


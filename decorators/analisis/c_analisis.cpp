#include <chrono>

#include "c_analisis.hpp"

//получение тактов процессора
uint64_t CycleCounter::rdtsc() {
        #ifdef __x86_64__
        unsigned int lo, hi;
        __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
        return ((uint64_t)hi << 32) | lo;
        #else
        return 0;
        #endif
}
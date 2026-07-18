#include <chrono>

class CycleCounter {
public:
    static uint64_t rdtsc();
};
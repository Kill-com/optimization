#pragma once

#include <iostream>
#include <chrono>
#include <functional>
#include <algorithm>
#include <unistd.h>      // syscall(), close()
#include <cstring> 
#if __linux__
    #include <sys/ioctl.h>   // ioctl() для управления счетчиками
    #include <linux/perf_event.h>  // Константы PERF_TYPE_*, PERF_COUNT_*
    #include <asm/unistd.h>  // __NR_perf_event_open (номер системного вызова)
    #include <perfmon/pfmlib.h>
    #include <perfmon/pfmlib_perf_event.h>
#endif

#include "../container/container.hpp"
#include "../controller/logger_controller/handler.hpp"

class CycleCounter{
private:
    uint64_t info;
    uint64_t rdtsc();
public:
    template<typename Ret, typename... Args>
    std::function<Ret(Args...)> prof_cycle(Ret (*func)(Args...)) {
        return [this,func](Args... args) -> Ret {
            info = CycleCounter::rdtsc();
            auto result =func(args...);
            info = CycleCounter::rdtsc() - info;
            return result;
        };
    }

    void getcycles();
};

class ProfilerFunctions{
private:
    inline static uint64_t info=0;
public:
    template<typename Ret, typename... Args>
    std::function<Ret(Args...)> prof_function(Ret (*func)(Args...)) {
        return [func](Args... args) -> Ret {
            ++info;
            return func(args...);
        };
    }
    void getcount(){
        std::stringstream ss;
        ss<<"\n=================================\n"
        <<"Function count: "<<info;
        logger->info(ss.str());
    };
};

class ConteinerPerf{
protected:
    struct Counter_ {
        int fd;              // Файловый дескриптор счетчика
        long long result;    // Результат измерения
        std::string name;    // Имя счетчика
    };
    std::vector<Counter_> counters;  // Все счетчики
    bool running = false;           // Состояние (запущены/остановлены)
    bool add_counter(const std::string&);
    long long get_result(const std::string&) const;
    void getperf();
};
class ProfilerPerf:public ConteinerPerf{
public:
    ProfilerPerf(){
        add_counter("task-clock"); 
        add_counter("page-faults");
        add_counter("PERF_COUNT_SW_CONTEXT_SWITCHES");
        add_counter("msr/pperf/");
    }
    void start();
    void stop();
    template<typename Ret, typename... Args>
    std::function<Ret(Args...)> prof_perf(Ret (*func)(Args...)) {
        return [this,func](Args... args) -> Ret {
            this->start();
            auto result =func(args...);
            this->stop();
            return result;
        };
    }
    ~ProfilerPerf(){
        for (auto& c : counters) {
            if (c.fd != -1) {
                close(c.fd);
            }
        }
    }
};

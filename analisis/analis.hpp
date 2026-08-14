#pragma once

#include <iostream>
#include <chrono>
#include <functional>
#include <algorithm>
#include <unistd.h>      // syscall(), close()
#include <cstring> 
#include <sstream>
#if __linux__
    #include <sys/ioctl.h>   // ioctl() для управления счетчиками
    #include <linux/perf_event.h>  // Константы PERF_TYPE_*, PERF_COUNT_*
    #include <asm/unistd.h>  // __NR_perf_event_open (номер системного вызова)
    #include <perfmon/pfmlib.h>
    #include <perfmon/pfmlib_perf_event.h>
#endif

#include "../container/container.hpp"
#include "../controller/logger_controller/handler.hpp"

//базовый класс логируемых объектов
class ToLog{
public:
    virtual ~ToLog()=default;
    virtual void tolog()=0;
    virtual void reset()=0;
};

//класс счетчик циклов
class CycleCounter:public ToLog{
private:
    uint64_t info=0;
    uint64_t rdtsc();
public:
//метод делает из данной функции функцию, измеряющую количество циклов
    template<typename Ret, typename... Args>
    std::function<Ret(Args...)> prof_cycle(std::function<Ret(Args...)>& func) {
        return [this,func](Args... args) -> Ret {
            info = CycleCounter::rdtsc();
            auto result =func(args...);
            info = CycleCounter::rdtsc() - info;
            return result;
        };
    }
//перегрузка для указателей на функции
    template<typename Ret, typename... Args>
    std::function<Ret(Args...)> prof_cycle(Ret (*func)(Args...)) {
        return [this,func](Args... args) -> Ret {
            info = CycleCounter::rdtsc();
            auto result =func(args...);
            info = CycleCounter::rdtsc() - info;
            return result;
        };
    }
    void reset(){};
    void tolog();
};

//класс подсчета кол-ва вызовов функций
class ProfilerFunctions:public ToLog{
private:
    inline static uint64_t info=0;
public:
//оборачиваем функцию на подсчет вызовов
    template<typename Ret, typename... Args>
    std::function<Ret(Args...)> prof_function(Ret (*func)(Args...)) {
        return [func](Args... args) -> Ret {
            ++info;
            return func(args...);
        };
    }
    //вывод в лог
    void tolog(){
        std::stringstream ss;
        ss<<"\n=================================\n"
        <<"Function count: "<<info;
        logger->info(ss.str());
    };
    //сброс счетчика
    void reset(){
        info=0;
    }
};

//Базовый класс для работы с аппаратными счетчиками производительности
class ConteinerPerf:public ToLog{
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
public:
    void tolog();
    void reset(){};
};

// Класс-профилировщик, использующий аппаратные счетчики производительности
//добавляет методы для управления счетчиками
class ProfilerPerf:public ConteinerPerf{
public:
    ProfilerPerf(){}
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
//ДЕСТРУКТОР!!!!!!!!! закрывает все дескрипторы счетчиков
    ~ProfilerPerf(){
        std::cout << "=== ProfilerPerf DESTRUCTOR ===" << std::endl;
        std::cout << "  this = " << this << std::endl;
        
        for (auto& c : counters) {
            std::cout << "  Closing: " << c.name << " (fd=" << c.fd << ")" << std::endl;
            if (c.fd != -1) {
                close(c.fd);
                c.fd = -1;
            }
        }
    }
};

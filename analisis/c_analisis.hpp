#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>      // syscall(), close()
#include <cstring> 
#include <functional>
#if __linux__
    #include <sys/ioctl.h>   // ioctl() для управления счетчиками
    #include <linux/perf_event.h>  // Константы PERF_TYPE_*, PERF_COUNT_*
    #include <asm/unistd.h>  // __NR_perf_event_open (номер системного вызова)
    #include <perfmon/pfmlib.h>
    #include <perfmon/pfmlib_perf_event.h>
#endif


class CycleCounter{
    static uint64_t rdtsc();
    public:
    template<typename Ret, typename... Args>
    static std::function<Ret(Args...)> call(Ret (*func)(Args...)) {
        return [func](Args... args) -> Ret {
            auto start = CycleCounter::rdtsc();
            auto result =func(args...);
            auto end = CycleCounter::rdtsc();
            auto cycles = end - start;
            std::cout<<cycles<<std::endl;
            return result;
        };
    }
    
};

class Container_Counter{
    protected:
    struct Counter_ {
        int fd;              // Файловый дескриптор счетчика
        long long result;    // Результат измерения
        std::string name;    // Имя счетчика
    };
    std::vector<Counter_> counters;  // Все счетчики
    bool running = false;           // Состояние (запущены/остановлены)
    public:
    long long count=0;
    virtual bool add_counter(const std::string&)=0;
    virtual ~Container_Counter()=default;
    long long get_result(const std::string&) const;
    friend std::ostream& operator<<(std::ostream&, const Container_Counter&);
};

class Start_Counter{
    public:
    virtual void start()=0;
    virtual ~Start_Counter()=default;
};    
class Stop_Counter{
    public:
    virtual void stop()=0;
    virtual ~Stop_Counter()=default;  
};

class Profiler_target_f{
    public:
    template<typename Ret, typename... Args, typename Class_container>
    static std::function<Ret(Args...)> count_target_f(Ret (*func)(Args...), Class_container* profiler) {
        return [func, profiler](Args... args) -> Ret {
            ++profiler->count;
            return func(args...);
        };
    }
    template<typename Target_Func, typename Class_container>
    static std::function<Target_Func> count_target_f(Target_Func func, Class_container* profiler){
        return [func, profiler](auto&&... args) -> decltype(auto) {
            ++profiler->count;
            return func(std::forward<decltype(args)>(args)...);
        };
    }
};
class Profiler:public Container_Counter, public Profiler_target_f{
    public:
    bool add_counter(const std::string& m){return true;};
};

class Profiler_linux:public Container_Counter, public Start_Counter, public Stop_Counter, public Profiler_target_f{
    public:
    Profiler_linux(){
        // assembling
        add_counter("task-clock"); 
        add_counter("page-faults");
        add_counter("PERF_COUNT_SW_CONTEXT_SWITCHES");
        add_counter("msr/pperf/");
    };
    bool add_counter(const std::string&);
    void start();
    void stop();
    ~Profiler_linux(){
        for (auto& c : counters) {
            if (c.fd != -1) {
                close(c.fd);
            }
        }
    }
};
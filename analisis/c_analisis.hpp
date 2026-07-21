#include <chrono>
#include <iostream>
#include <string>
#include <vector>
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

class CycleCounter {
public:
    static uint64_t rdtsc();
};

class Counter{
    protected:
    struct Counter_ {
        int fd;              // Файловый дескриптор счетчика
        long long result;    // Результат измерения
        std::string name;    // Имя счетчика
    };
    std::vector<Counter_> counters;  // Все счетчики
    bool running = false;           // Состояние (запущены/остановлены)
    public:
    virtual ~Counter()=default;
    virtual bool add_counter(const std::string&)=0;
    virtual void start()=0;
    virtual void stop()=0;
    long long get_result(const std::string&) const;
    friend std::ostream& operator<<(std::ostream&, const Counter&);
};
    
class Profiler:public Counter{
    public:
    long long count=0;
    // template<typename Methods, typename Functions, typename... Args>
    // Profiler(Methods m, Functions f,Args&&... args){
    // }
    // количество вычеслений целевой функции
      // Принимает указатель на функцию, возвращает std::function с той же сигнатурой
    template<typename Ret, typename... Args>
    static std::function<Ret(Args...)> count_target_f(Ret (*func)(Args...), Profiler* profiler) {
        return [func, profiler](Args... args) -> Ret {
            ++profiler->count;
            return func(args...);
        };
    }
    template<typename Target_Func>
    static std::function<Target_Func> count_target_f(Target_Func func, Profiler* profiler){
        return [func, profiler](auto&&... args) -> decltype(auto) {
            ++profiler->count;
            return func(std::forward<decltype(args)>(args)...);
        };
    }
    bool add_counter(const std::string& m){return true;};
    void start(){};
    void stop(){};
};

class Profiler_linux: public Counter{
    public:
    Profiler_linux(){
        // assembling
        add_counter("task-clock"); 
        add_counter("page-faults");
        add_counter("PERF_COUNT_SW_CONTEXT_SWITCHES");
        // add_counter("msr/pperf/");
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
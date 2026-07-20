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

long long Counter::get_result(const std::string& name) const{
        for (const auto& c : counters) {
            if (c.name == name) {
                return c.result;
            }
        }
        std::cerr<<"Name not found"<<std::endl;
        return -1;
}

bool Profiler_linux::add_counter(const std::string& name){
        // 1. Инициализация библиотеки (вызывается один раз в программе)
        static bool initialized = []() {
            return pfm_initialize() == PFM_SUCCESS;
        }();
        if (!initialized) return false;

        // 2. Подготавливаем аргументы для преобразования
        struct perf_event_attr attr = {};
        pfm_perf_encode_arg_t arg = {};
        arg.size = sizeof(pfm_perf_encode_arg_t);
        arg.attr = &attr;

        // 3. Преобразуем строку "msr/pperf/" в структуру attr
        // PFM_PLM3 означает измерение на уровне пользователя
        int ret = pfm_get_os_event_encoding(name.c_str(), PFM_PLM3,
                                            PFM_OS_PERF_EVENT_EXT, &arg);

        if (ret != PFM_SUCCESS) {
            std::cerr << "Ошибка разбора события " << name << ": "
                      << pfm_strerror(ret) << std::endl;
            return false;
        }

        // 4. Создаем счетчик с автоматически заполненным attr
        Counter_ c;
        c.name = name;
        c.result = 0;
        // attr.type будет равен PERF_TYPE_RAW
        // attr.config будет содержать правильный код события
        int group_fd = counters.empty() ? -1 : counters[0].fd;
        c.fd = perf_event_open(&attr, 0, -1, group_fd, 0);
        
        if (c.fd != -1) {
                counters.push_back(c);
            std::cout << "Добавлен счетчик: " << name << std::endl;
            return true;
        } else {
            std::cerr << "Не удалось добавить счетчик: " << name << std::endl;
            return false;
        }
}
void Profiler_linux::start(){
        if (running) {
            std::cerr << "Error, Счетчики уже запущены!" << std::endl;
            return;
        }
        
        for (auto& c : counters) {
            ioctl(c.fd, PERF_EVENT_IOC_RESET, 0);
            ioctl(c.fd, PERF_EVENT_IOC_ENABLE, 0);
        }
        running = true;
}

void Profiler_linux::stop(){
        if (!running) {
                std::cerr << "Error, Счетчики уже остановлены!" << std::endl;
                return;
        }

        for (auto& c : counters) {
                ioctl(c.fd, PERF_EVENT_IOC_DISABLE, 0);
                read(c.fd, &c.result, sizeof(c.result));
        }
        running = false;
}


std::ostream& operator<<(std::ostream& os, const Counter& counter){
         for (const auto& c : counter.counters) {
            os << "  " << c.name << ": " << c.result;
        }
        return os;
}


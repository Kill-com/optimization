
#include <sstream>

#include "analis.hpp"

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

void CycleCounter::tolog(){
    std::stringstream ss;
    ss<<"\n=================================\n"
    <<"Cycle count: "<<info;
    logger->info(ss.str());
}
#if __linux__
long long ConteinerPerf::get_result(const std::string& name) const{
    for (const auto& c : counters) {
        if (c.name == name) {
            return c.result;
        }
    }
    std::cerr<<"Name not found"<<std::endl;
    return -1;
}

bool ConteinerPerf::add_counter(const std::string& name){
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

void ConteinerPerf::tolog(){
    std::stringstream ss;
    ss<<"\n=================================\n"
    <<"Perf result:\n ";
    for (auto& c : counters) {
        ss << "  " << c.name << ": " << c.result;
    }
    logger->info(ss.str());
}
void ProfilerPerf::start(){
    add_counter("task-clock"); 
    add_counter("page-faults");
    add_counter("PERF_COUNT_SW_CONTEXT_SWITCHES");
    add_counter("msr/pperf/");
    if (running) {
        std::cerr << "Error: Счетчики уже запущены!" << std::endl;
        return;
    }
    
    std::cout << "Запуск счетчиков..." << std::endl;
    for (auto& c : counters) {
        if (c.fd == -1) {
            std::cerr << "Warning: Счетчик " << c.name << " не инициализирован (fd=-1)" << std::endl;
            continue;  // <-- Пропускаем невалидные счетчики
        }
        ioctl(c.fd, PERF_EVENT_IOC_RESET, 0);
        ioctl(c.fd, PERF_EVENT_IOC_ENABLE, 0);
    }
    running = true;
}

void ProfilerPerf::stop(){
    if (!running) {
        std::cerr << "Error: Счетчики уже остановлены!" << std::endl;
        return;
    }

    std::cout << "Остановка счетчиков..." << std::endl;
    for (auto& c : counters) {
        if (c.fd == -1) {
            std::cerr << "Warning: Счетчик " << c.name << " не инициализирован (fd=-1)" << std::endl;
            continue;  // <-- Пропускаем невалидные счетчики
        }
        
        ioctl(c.fd, PERF_EVENT_IOC_DISABLE, 0);
        ssize_t bytes_read = read(c.fd, &c.result, sizeof(c.result));
        if (bytes_read != sizeof(c.result)) {
            std::cerr << "Error: Не удалось прочитать счетчик " << c.name 
                      << " (bytes_read=" << bytes_read << ", errno=" << errno 
                      << ": " << strerror(errno) << ")" << std::endl;
            c.result = -1;
        }
    }
    running = false;
}

#endif
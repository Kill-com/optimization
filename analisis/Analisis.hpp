
#include <iostream>

#include "../exect/execution.hpp"
#include "c_analisis.hpp"

// ============================================
// ОПРЕДЕЛЕНИЕ ОПЕРАЦИОННОЙ СИСТЕМЫ
// ============================================
#ifdef _WIN32
    #define IS_WINDOWS 1
    #define IS_LINUX 0
    #define PLATFORM_NAME "Windows"
#elif __linux__
    #define IS_WINDOWS 0
    #define IS_LINUX 1
    #define PLATFORM_NAME "Linux"
#else
    #define IS_WINDOWS 0
    #define IS_LINUX 0
    #define PLATFORM_NAME "Unknown"
#endif

//класс вызова анализируемой функции
class EXECTED : protected METHOD_L{
    private:
    template<typename Methods, typename Functions, typename... Args>
    void analis(Methods m, Functions f,Args&&... args){
        std::cout<<"start of analisis"<<std::endl;

        //Получаем начальный такт процессора
        auto start = CycleCounter::rdtsc();

        //Запуск основной работы
        exect(m,f,std::forward<Args>(args)...);
        
        //Получаем конечный такт процессора
        auto end = CycleCounter::rdtsc();
        auto cycles = end - start;
        std::cout<<"end of analisis"<<std::endl;
        std::cout<<cycles<<std::endl;


        std::cout<<"save results in created file"<<std::endl;

        // return result;
    }
    template<typename Methods, typename Functions, typename... Args>
    void analis_linux(Methods m, Functions f,Args&&... args){
        //анализ процессора с помощью линукса
        exect(m,f,std::forward<Args>(args)...);
        //конец анализа с помощью линукса
    }
    public:
    EXECTED(std::string m = "") : METHOD_L(m){};

    template<typename... Args>
    void operator()(std::string f,Args&&... args){
        auto exect_wrapper = [this](auto process, auto process_f, auto&&... wrapped_args) {
            #if IS_LINUX
            // На Linux
                this->analis_linux(process, process_f, std::forward<decltype(wrapped_args)>(wrapped_args)...);
            #else
                this->analis(process, process_f, std::forward<decltype(wrapped_args)>(wrapped_args)...);
            };
            #endif
        collect(exect_wrapper,f,std::forward<Args>(args)...);
    };
};

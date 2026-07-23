
#include <iostream>

#include "../exect/execution.hpp"
#include "c_analisis.hpp"

//класс вызова анализируемой функции
class EXECTED : protected METHOD_L{
    private:
    template<typename Methods, typename Functions, typename... Args>
    void analis(Methods m, Functions f,Args&&... args){
        std::cout<<"start of analisis"<<std::endl;
        Profiler profiler;
        auto count_target=Profiler::count_target_f(f,&profiler);
        //Запуск основной работы
        auto m_=CycleCounter::call(m);
        exect(m_,count_target,std::forward<Args>(args)...);
        std::cout<<profiler.count<<std::endl;


        std::cout<<"save results in created file"<<std::endl;

        // return result;
    }
    template<typename Func,typename... Args>
    void analis_linux(Func func, Args&&... args){
        //анализ процессора с помощью линукса
        Profiler_linux profiler;
        profiler.start();
        func();
        profiler.stop();
        std::cout<<profiler<<std::endl;
        //конец анализа с помощью линукса
    }
    public:
    EXECTED(std::string m = "") : METHOD_L(m){};

    template<typename... Args>
    void operator()(std::string f,Args&&... args){
        auto exect_wrapper = [this](auto process, auto process_f, auto&&... wrapped_args) {
            #if __linux__
            // На Linux
                this->analis_linux(process, process_f, std::forward<decltype(wrapped_args)>(wrapped_args)...);
            #else
                this->analis(process, process_f, std::forward<decltype(wrapped_args)>(wrapped_args)...);
            #endif
            };
        collecting(exect_wrapper,methods,f,std::forward<Args>(args)...);
    };
};







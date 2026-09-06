#pragma once

#include <memory>
#include "analis.hpp"

#include "../controller/start_controller/execution.hpp"
#include "../controller/start_controller/interface.hpp"

//шаблонный класс для анализа
//наследует выполнение функций, счетчик циклов и измерение времени выполнения
template<typename ...Args>
class IAnalis: public SimpleExect<Args...>{
private:
    std::shared_ptr<CycleCounter> counter;
    std::shared_ptr<ProfilerFunctions> profiler;
    auto getwrapped(){
        return [this](auto&& ...args){
            return SimpleExect<Args...>::exect(
                std::forward<decltype(args)>(args)...
            );
        };
    }
public:
    IAnalis(ContainerValue<Args...>& vl) 
        : SimpleExect<Args...>(vl),
          counter(std::make_shared<CycleCounter>()),
          profiler(std::make_shared<ProfilerFunctions>()) {}

    template<typename Method,typename... Func>
    void exect(Method&& method,Func&&... func){
        ConteinerLog::input_command(counter);
        ConteinerLog::input_command(profiler);
        std::cout<<"start of analisis"<<std::endl;
        auto method_cycles=counter->prof_cycle(method);
        auto methods_funcs=std::make_tuple(
            method_cycles,
            profiler->prof_function(std::forward<Func>(func))...
        );
        std::apply(
            getwrapped(),
            methods_funcs
        );
        ConteinerLog::startlog();
        ConteinerLog::reset();
    }
};

//анализ на винде
template<typename ...Args>
class WindowAnalis:public IAnalis<Args...>{
public:
    using IAnalis<Args...>::IAnalis;
};

//анализ на линуксе
template<typename ...Args>
class LinuxAnalis:protected IAnalis<Args...>{
private:
    std::shared_ptr<ProfilerPerf> perf;
public:
    LinuxAnalis(ContainerValue<Args...>& vl) 
        : IAnalis<Args...>(vl),
          perf(std::make_shared<ProfilerPerf>()) {}
    template<typename Method,typename... Func>
    void exect(Method&& method,Func&&... func){
        ConteinerLog::input_command(perf);
        IAnalis<Args...>::exect(
            perf->prof_perf(method),
            std::forward<Func>(func)...
        );
    }
};

//класс для выбоора реализации в зависимости от операционки
template<typename ...Args>
class AnalisFactory:public
    #ifdef _WIN32
        WindowAnalis<Args...>
    #elif __linux__
        LinuxAnalis<Args...>
    #else
        #error "Unsupported platform";
    #endif
{
public:
    #ifdef _WIN32
        using WindowAnalis<Args...>::WindowAnalis;
    #elif __linux__
        using LinuxAnalis<Args...>::LinuxAnalis;
    #endif
};

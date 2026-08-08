#pragma once

#include <memory>
#include "analis.hpp"

#include "../controller/start_controller/execution.hpp"
#include "../controller/start_controller/interface.hpp"

template<typename ...Args>
class AnalisExect:public IExect<Args...>{
public:
    using IExect<Args...>::IExect;
    template<typename Method,typename... Func>
    void operator()(Method&& method,Func&&... func){
        this->unpacking_tuple(
            this->value.getinfo(),
            std::forward<Func>(func)...
        );
    }
};
template<typename ...Args>
class IAnalis: public SimpleExect<Args...>,
    public CycleCounter, public ProfilerFunctions
{
private:
    auto getwrapped(){
        return [this](auto&& ...args){
            return SimpleExect<Args...>::operator()(
                std::forward<decltype(args)>(args)...
            );
        };
    }
public:
    using SimpleExect<Args...>::SimpleExect;
    template<typename Method,typename... Func>
    void operator()(Method&& method,Func&&... func){
        std::cout<<"start of analisis"<<std::endl;
        auto method_cycles=prof_cycle(method);
        auto methods_funcs=std::make_tuple(
            method_cycles,
            prof_function(std::forward<Func>(func))...
        );
        std::apply(
            getwrapped(),
            methods_funcs
        );
        getcycles();
        getcount();
    }
};
template<typename ...Args>
class WindowAnalis:public IAnalis<Args...>{
public:
    using IAnalis<Args...>::IAnalis;
};

template<typename ...Args>
class LinuxAnalis:public IAnalis<Args...>,
    public ProfilerPerf
{
public:
    using IAnalis<Args...>::IAnalis;
    template<typename Method,typename... Func>
    void operator()(Method&& method,Func&&... func){
        IAnalis<Args...>::operator()(
            prof_perf(method),
            std::forward<Func>(func)...
        );
    }
};

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

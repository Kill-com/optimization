#pragma once

#include <memory>
#include "analis.hpp"

#include "../controller/start_controller/execution.hpp"
#include "../controller/start_controller/interface.hpp"

class ConteinerLog{
private:
    static std::vector<ToLog*> commands;  // Только сырые указатели

public:
    // Для передачи существующих объектов в список
    template<class T>
    static void input_command(T* cl) {
        if (cl) {
            commands.push_back(cl);
        }
    }
    
    // Для создания новых объектов (НЕ ИСПОЛЬЗУЙТЕ С UNIQUE_PTR!)
    template<class T, typename... Args>
    static void add_command(Args&&... args) {
        commands.push_back(new T(std::forward<Args>(args)...));
    }
    
    static void reset(){
        // ВНИМАНИЕ: Удаляем объекты, если они были созданы через add_command
        for (auto* cmd : commands) {
            cmd->reset();
        }
        commands.clear();
    }
    
    //старт логирования
    static void startlog(){
        for (auto* cmd : commands) {
            if (cmd) {
                cmd->tolog();
            }
        }
    }
};
std::vector<ToLog*> ConteinerLog::commands;


//шаблонный класс для анализа
//наследует выполнение функций, счетчик циклов и измерение времени выполнения
template<typename ...Args>
class IAnalis: public SimpleExect<Args...>,
    public CycleCounter, public ProfilerFunctions
{
private:
    auto getwrapped(){
        return [this](auto&& ...args){
            return SimpleExect<Args...>::exect(
                std::forward<decltype(args)>(args)...
            );
        };
    }
public:
    using SimpleExect<Args...>::SimpleExect;
    template<typename Method,typename... Func>
    void exect(Method&& method,Func&&... func){
        ConteinerLog::input_command(static_cast<CycleCounter*>(this));
        ConteinerLog::input_command(static_cast<ProfilerFunctions*>(this));
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
class LinuxAnalis:protected IAnalis<Args...>,
    public ProfilerPerf
{
public:
    using IAnalis<Args...>::IAnalis;
    template<typename Method,typename... Func>
    void exect(Method&& method,Func&&... func){
        ConteinerLog::input_command(static_cast<ProfilerPerf*>(this));
        IAnalis<Args...>::exect(
            prof_perf(method),
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

#pragma once

#include "interface.hpp"

template<typename ...Args>
class SimpleExect:public IExect<Args...>{
public:
    using IExect<Args...>::IExect;
    template<typename... Func>
    void operator()(Func&&... func){
        this->unpacking_tuple(
            this->value.getinfo(),
            std::forward<Func>(func)...
        );
    }
};

template<template<typename...> class Classes, typename ...Args>
class StartCalculation:public ICollectStart, public IStartCalculation{
private:
    containerValue<Args...> value;
public:
    StartCalculation(){};
    template<typename ...Func>
    StartCalculation(std::string method_name, Func&&... function_name):
        ICollectStart(method_name,std::forward<Func>(function_name)...)
    {};
    template<typename ...U>
    void input_value(U&&...u){
        value=containerValue<Args...>(std::forward<U>(u)...);
    }
    void operator()(){
        StartPlug<Classes<Args...>> start_plug(value, target_function);
        std::string method_name = method;
        start_plug(method_name);
    }
};
#pragma once

#include "interface.hpp"

template<typename ...Args>
class SimpleExect:public IExect<Args...>{
public:
    using IExect<Args...>::IExect;
    template<typename... Func>
    void exect(Func&&... func){
        this->unpacking_tuple(
            this->value.getinfo(),
            std::forward<Func>(func)...
        );
    }
};

/**
 * @tparam Classes Класс в котором происходит основной вызов exect
 * @tparam Args Типы аргументов value
 */
template<template<typename...> class Classes, typename ...Args>
class StartCalculation:public ICollectStart{
private:
    ContainerValue<Args...> value; //Хранение value
    using FirstType = typename std::tuple_element<0, std::tuple<Args...>>::type; //Получение первого типа в Args
public:
    //конструктор по умолчанию
    StartCalculation(){};
    /**
     * @brief Construct a new Start Calculation object
     * 
     * @param method_name Имя метода
     * @param function_name Имена функций
     */
    template<typename ...Func>
    StartCalculation(std::string method_name, Func&&... function_name):
        ICollectStart(method_name,std::forward<Func>(function_name)...)
    {};
    /**
     * @brief Сохранение любых аргументов в value для вызова
     * Они используютсья при вызове как начальные точки
     * 
     * @param u аргументы
     */
    template<typename ...U>
    void input_value(U&&...u){
        value=ContainerValue<Args...>(std::forward<U>(u)...);
    }
    /**
     * @brief Вызывает метод с его функциями и аргументами
     * 
     */
    void operator()(){
        StartPlug<Classes<Args...>> startplug(value, target_function);
        std::string method_name = method;
        startplug.template start_plug<FirstType>(method_name);
    }
};
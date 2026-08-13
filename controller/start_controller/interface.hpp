#pragma once

#include "../../container/container.hpp"
#include "../assembler_controller/plugins_controller.hpp"

/**
 * @brief КлассХранилище для строк
 */
class ICollectStart{
protected:
    containerStr method;
    containerVectorStr target_function;
public:
    /**
     * @brief Construct a new ICollectStart object
     * 
     * @param method_name Имя метода
     * @param function_name Имена вспомогательных-целевых функций
     */
    template<typename ...Args>
    ICollectStart(std::string method_name, Args&&... function_name):
    method(containerStr(method_name)),
    target_function(containerVectorStr(std::forward<Args>(function_name)...))
    {}
};
/**
 * @brief Методы вызова с tuple переменными
 * Хранение value для метода
 * 
 * @tparam Args Типы Value для вызова метода отличные от имен функций
 */
template<typename ...Args>
class IExect{
private:
    /**
     * @brief Get the wrapped start object
     * 
     * @return лямбду которая вызывает основной метод
     */
    auto getwrapped_start(){
        return [](auto&& method, auto&&...args){
            if constexpr (std::is_invocable_v<decltype(method), decltype(args)...>){
                return method(std::forward<decltype(args)>(args)...);
            }
        };
    }
protected:
    ContainerValue<Args...> value;
    /**
     * @brief Распаковка tuple и вызов начальной лямбды
     * 
     * @param tuple Аргументы
     * @param func Указатели на функции где первый заувисит от следующих
     */
    template<typename... Plugins, typename Tuple>
    void unpacking_tuple(Tuple&& tuple,Plugins&&...func){
        auto all_args = std::tuple_cat(
            std::tuple(std::forward<Plugins>(func))...,
            std::forward<Tuple>(tuple)
        );
        std::apply(getwrapped_start(), all_args);
    }
public:
    /**
     * @brief Конструктор копирования для value
     * 
     * @param vl Класс котоырй хранит в себе value
     */
    IExect(ContainerValue<Args...>& vl):value(vl){};
};
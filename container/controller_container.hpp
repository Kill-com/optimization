#pragma once

#include <iostream>

#include <tuple>

#include "../controller/derived_controller.hpp"
#include "../controller/logger_controller/interface.hpp"
#include "../controller/logger_controller/handler.hpp"
#include "../analisis/name_iter.hpp"

template<template<typename...> class Derived, typename T>
class ControlVector:public ControlDerived<Derived<T>>{
public:
//смотрим размер вектора
    size_t getsize(){
        return this->derived_ptr->getinfo().size();
    }
//доступ к элементам вектора по индексу
    T operator[](size_t index) {
        return this->derived_ptr->getinfo()[index];
    }
};

//класс для работы с кортежами
class ControlTuple{
public:
template<typename T>

//приведение к кортежу
static auto to_tuple(T&& t) {
    if constexpr (std::is_same_v<std::decay_t<T>, std::tuple<>>) {
        return std::forward<T>(t);
    } else {
        return std::tuple(std::forward<T>(t));
    }
}

//перегрузка to_tuple для rvalue ссылок (просто пересылает кортеж дальше, не изменяяя)
template<typename... Ts>
static auto to_tuple(std::tuple<Ts...>&& t) {
    return std::forward<std::tuple<Ts...>>(t);
}
};

template<typename Derived, auto Name>
class ControlCount: public ControlDerived<Derived>, public ToLog{
public:
    // 1. ПРЕФИКС: ++x
    Derived& operator++() {
        ++this->derived_ptr->getinfo();    // 1. Изменяем себя
        return *this->derived_ptr;    // 2. Возвращаем себя (ссылку)
    }
    void tolog() {
        logger->template info<Name>(std::to_string(this->derived_ptr->getinfo()));
    }
    void reset(){
        this->derived_ptr->inputinfo(0);
    }
};
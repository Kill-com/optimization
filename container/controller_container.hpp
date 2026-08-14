#pragma once

#include <tuple>

#include "../controller/derived_controller.hpp"

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
class ConrolTuple{
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
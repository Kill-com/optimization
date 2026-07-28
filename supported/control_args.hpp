#pragma once
#include <cstddef>
#include <utility>
#include <tuple>
#include <type_traits>
#include <array>
#include <optional>
#include <algorithm>
#include <variant>
#include <memory>

#include "tuple_control.hpp"

template<typename T>
struct unwrap_optional {
    using type = T;
};

template<typename T>
struct unwrap_optional<std::optional<T>> {
    using type = T;  // Достаем T из optional
};

template<typename T>
using unwrap_optional_t = typename unwrap_optional<std::decay_t<T>>::type;


template<typename T>
struct is_tuple : std::false_type {};

template<typename... Ts>
struct is_tuple<std::tuple<Ts...>> : std::true_type {};

template<typename T>
constexpr bool is_tuple_v = is_tuple<std::decay_t<T>>::value;


template<typename T>
struct is_optional : std::false_type {};

template<typename T>
struct is_optional<std::optional<T>> : std::true_type {};

template<typename T>
inline constexpr bool is_optional_v = is_optional<std::decay_t<T>>::value;

// Вспомогательная функция для проверки, пустой ли optional
template<typename T>
bool is_optional_empty(const T& value) {
    if constexpr (is_optional_v<T>) {
        return !value.has_value();
    }
    return false;
}

namespace MakeClasses{
template<typename Classes, typename... Args>
auto makeclass(Args&&... args) {
    return Classes(std::forward<Args>(args)...);
}
template<template<typename...> class Classes, typename... Args>
auto makeclass(Args&&... args) {
    return Classes<unwrap_optional_t<Args>...>(
        std::forward<Args>(args)...
    );
}
};

namespace AssemArg {
// Основная функция
template<typename T>
auto to_tuple(T&& arg);

// Вспомогательный тип для определения, является ли тип обычным (не tuple и не optional)
template<typename T>
struct is_plain : std::true_type {};

template<typename... Ts>
struct is_plain<std::tuple<Ts...>> : std::false_type {};

template<typename T>
struct is_plain<std::optional<T>> : std::false_type {};

// Для обычных типов (включая указатели на функции)
template<typename T>
auto to_tuple_impl(T&& arg, std::true_type) {
    return std::tuple<std::decay_t<T>>(std::forward<T>(arg));
}

// Для optional (rvalue)
template<typename T>
auto to_tuple_impl(std::optional<T>&& arg, std::false_type) {
    if (arg.has_value()) {
        return std::tuple<T>(std::move(*arg));
    } else {
        return std::tuple<>{};
    }
}

// Для optional (const lvalue)
template<typename T>
auto to_tuple_impl(const std::optional<T>& arg, std::false_type) {
    if (arg.has_value()) {
        return std::tuple<T>(*arg);
    } else {
        return std::tuple<T>{};
    }
}

// Для tuple (rvalue)
template<typename... Ts>
auto to_tuple_impl(std::tuple<Ts...>&& arg, std::false_type) {
    return std::apply(
        [](auto&&... elems) {
            return std::tuple_cat(
                to_tuple(std::forward<decltype(elems)>(elems))...
            );
        },
        std::forward<std::tuple<Ts...>>(arg)
    );
}

// Для tuple (const lvalue)
template<typename... Ts>
auto to_tuple_impl(const std::tuple<Ts...>& arg, std::false_type) {
    return std::apply(
        [](const auto&... elems) {
            return std::tuple_cat(
                to_tuple(elems)...
            );
        },
        arg
    );
}

// Основная функция
template<typename T>
auto to_tuple(T&& arg) {
    using DecayedT = std::decay_t<T>;
    return to_tuple_impl(
        std::forward<T>(arg),
        is_plain<DecayedT>{}  // true для обычных типов, false для tuple и optional
    );
}

// Основная функция assembling_args
template<typename Func, typename... Args>
auto assembling_args(Func&& func, Args&&... args) {
    // Преобразуем каждый аргумент в кортеж и объединяем
    auto all_args = std::tuple_cat(
        to_tuple(std::forward<Args>(args))...
    );
    
    // Вызываем функцию с распакованными аргументами
    return std::apply(
        std::forward<Func>(func),
        all_args
    );
}
// для создания классов по tuple возвращает класс
template<template<typename...> class Classes>
auto assembling_args_class() {
    return [](auto&&... wrapped_args) {
        // Распаковываем аргументы перед созданием класса
        return MakeClasses::makeclass<Classes>(
            unwrap_optional_t<decltype(wrapped_args)>(
                std::forward<decltype(wrapped_args)>(wrapped_args)
            )...
        );
    };
}

template<class Classes>
auto assembling_args_class() {
    return [](auto&&... wrapped_args) {
        // Распаковываем аргументы перед созданием класса
        return MakeClasses::makeclass<Classes>(
            unwrap_optional_t<decltype(wrapped_args)>(
                std::forward<decltype(wrapped_args)>(wrapped_args)
            )...
        );
    };
}

} // namespace AssemArg

namespace GetExFi{
namespace {
    template<typename Tuple, size_t... Is>
    auto get_rest_impl(Tuple&& tupleArgs, std::index_sequence<Is...>){
        // Создаем копию кортежа
        auto result = std::forward<Tuple>(tupleArgs);
        
        // Ищем первый непустой optional через fold expression
        bool found = false;
        (
            [&]() {
                if (found) return;
                
                auto& elem = std::get<Is>(result);
                if constexpr (is_optional_v<std::decay_t<decltype(elem)>>) {
                    if (elem.has_value()) {
                        elem.reset();
                        found = true;
                    }
                }
            }(),
            ...
        );
        
        return result;
    }
}

template<size_t N,typename ... Args>
auto get_excludefirst(Args&&... args){
    static_assert(N > 0, "N must be > 0");
    // Остальные аргументы (со 2-го по N-й)
    auto tuple = std::forward_as_tuple(std::forward<Args>(args)...);
    constexpr size_t size = sizeof...(Args);
    return get_rest_impl(
        std::forward<decltype(tuple)>(tuple),
        std::make_index_sequence<size>{}
    );
}
};

template<typename... Args>
class StorArg{
private:
    template<typename TempTuple, size_t... Is>
    auto fill_from_end(TempTuple&& temp, std::index_sequence<Is...>) {
        constexpr size_t M = std::tuple_size_v<std::decay_t<TempTuple>>;
        constexpr size_t Start = N - M;  // Индекс, с которого начинаем заполнять
        
        return std::tuple<std::optional<Args>...>(
            // Если индекс >= Start - берем из temp, иначе - пустой optional
            (Is >= Start) ? std::get<Is - Start>(std::forward<TempTuple>(temp)) 
                          : std::optional<Args>()...
        );
    }
protected:
     std::tuple<std::optional<Args>...> all;
    static constexpr size_t N = sizeof...(Args);

public:
    StorArg() : all() {}
    template<typename... Args2>
    explicit StorArg(Args2&&... args) {
        constexpr size_t M = sizeof...(Args2);
        static_assert(M <= N, "Too many arguments");
        
        // Создаем tuple из переданных аргументов
        auto temp = std::tuple(
            std::optional<unwrap_optional_t<Args2>>(
                std::forward<Args2>(args)
            )...
        );
        
        // Заполняем с конца
        all = fill_from_end(temp, std::make_index_sequence<N>{});
    }
    size_t getN() const {return N;};

    // возвращает tuple
    auto getfirst() const{
        return std::get<0>(all);
    }

    auto getall() const {
        return std::apply(
            [](auto&&... elems) {
                return std::tuple<Args...>(
                    elems.has_value() ? *elems : Args{}...
                );
            },
            all
        );
    }

    auto get_excludefirst() const{
        if constexpr (N == 0) {
            return std::tuple<>{};  // Пустой tuple
        } else {
            return GetExFi::get_excludefirst<N>(all);
        }
    }
};

template<typename... Args>
class GetForInd: public StorArg<Args...>    {
private:
    using StorArg<Args...>::all;
    using StorArg<Args...>::N;

public:
    GetForInd() : StorArg<Args...>() {}
    template<typename... U>
    explicit GetForInd(U&&... u) : StorArg<Args...>(std::forward<U>(u)...) {}
};

namespace GetArgs{

template<typename T, typename ... Args>
auto getfirst(T&& t, Args&& ... args){
    return std::decay_t<T>(std::forward<T>(t));  // Создаем копию
}
namespace{
// Удаляет первый элемент из tuple класса
// возвращает класс
template<typename Classes>
auto delEl(Classes& storage){
    auto wrapper=AssemArg::assembling_args_class<StorArg>();
    return AssemArg::assembling_args(
        wrapper,
        storage.get_excludefirst()
    );
}

// Возвращает класс масив+первый элемент переданых аргументов
    template<typename StartClass, typename Manager, typename... Args>
    auto getfor_support(Manager&& manager, Args&&... args){
        using DecayedStartClass = std::decay_t<StartClass>;
        
        return AssemArg::assembling_args(
            AssemArg::assembling_args_class<DecayedStartClass>(),
            manager.getall(),
            getfirst(std::forward<Args>(args)...)
        );
    }

// Возвращает класс с таким же типом но меньшим значением
    template<typename Classes>
    auto storage_support(Classes&& storage){
        using DecayedClasses = std::decay_t<Classes>;
        
        return AssemArg::assembling_args(
            AssemArg::assembling_args_class<DecayedClasses>(),
            delEl(storage).getall()
        );
    }
}

//реализация функции для получения масива [1;N] из args
// Возвращает tuple
template<typename... Args>
auto getfor_i(int N, Args&&... args){
    using DecayedArgs = std::tuple<unwrap_optional_t<Args>...>;
    
    auto storage = MakeClasses::makeclass<StorArg>(
        std::forward<Args>(args)...
    );
    
    GetForInd<unwrap_optional_t<Args>...> manager;
    
    storage = storage_support(storage);
    for(int i = 1; i < N; i++){
        auto wrapped = [&manager](auto&&... wrapped_args){
            return getfor_support<decltype(manager)>(
                manager,
                std::forward<decltype(wrapped_args)>(wrapped_args)...
            );
        };
        manager = AssemArg::assembling_args(
            wrapped,
            storage.getall()
        );
        storage = storage_support(storage);
    }
    return manager.getall();
}
// получение масива аргументов с N+1 
// Возвращает tuple
// Вспомогательная функция для рекурсии
template<typename... Args>
auto getnotfor_i(int N, Args&&... args){
    auto storage=MakeClasses::makeclass<StorArg>(
        std::forward<Args>(args)...
    );
    for(int i=0; i<N;i++){
        storage=storage_support(storage);
    }
    return storage.getall();
}
};

template<typename... Args>
class ManagementArgs: public StorArg<Args...>{
private:
    using StorArg<Args...>::all;
    using StorArg<Args...>::N;
public:
    explicit ManagementArgs(Args... args) : StorArg<Args...>(std::forward<Args>(args)...){};

    auto getfor_i(int N){
        auto wrapper = [&](auto&&... args) {
            return GetArgs::getfor_i(N, std::forward<decltype(args)>(args)...);
        };
        return AssemArg::assembling_args(
            wrapper,
            this->all
        );
    }

    auto getnotfor_i(int N){
        auto wrapper = [&](auto&&... args) {
            return GetArgs::getnotfor_i(N, std::forward<decltype(args)>(args)...);
        };
        return AssemArg::assembling_args(
            wrapper,
            this->all
        );
    }
};

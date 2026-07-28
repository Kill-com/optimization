#pragma once
#include <tuple>
#include <type_traits>

// Проверка, является ли тип tuple-подобным
template<typename T>
struct is_tuple_like : std::false_type {};

template<typename... Ts>
struct is_tuple_like<std::tuple<Ts...>> : std::true_type {};

// Преобразуем любой аргумент в tuple
template<typename T>
auto as_tuple(T&& arg) {
    if constexpr (is_tuple_like<std::decay_t<T>>::value) {
        return std::forward<T>(arg);
    } else {
        return std::tuple<std::decay_t<T>>(std::forward<T>(arg));
    }
}

// Объединяем все в один tuple
template<typename... Args>
auto make_combined_tuple(Args&&... args) {
    return std::tuple_cat(as_tuple(std::forward<Args>(args))...);
}
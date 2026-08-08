#pragma once

#include <functional>

template<typename... Args>
struct count_function_args;

// === ОБЩАЯ ПРОВЕРКА НА std::function ===
template<typename T>
struct is_std_function : std::false_type {};

template<typename Ret, typename... Args>
struct is_std_function<std::function<Ret(Args...)>> : std::true_type {};

template<typename T>
constexpr bool is_std_function_v = is_std_function<std::decay_t<T>>::value;

// === ПОДСЧЕТ ДЛЯ ЛЮБОЙ ФУНКЦИИ ===
template<typename Func>
struct function_counter;

template<typename Ret, typename... Args>
struct function_counter<Ret(Args...)> {
    static constexpr int value = (0 + ... + (is_std_function_v<Args> ? 1 : 0));
};

template<typename Ret, typename... Args>
struct function_counter<Ret(*)(Args...)> {
    static constexpr int value = (0 + ... + (is_std_function_v<Args> ? 1 : 0));
};

template<typename Func>
constexpr int count_std_functions(Func&&) {
    using Decayed = std::decay_t<Func>;
    // Прямо используем специализацию для указателей
    return function_counter<Decayed>::value;
}
template<typename Func>
constexpr int count_std_functions_type() {
    return function_counter<std::decay_t<Func>>::value;
}
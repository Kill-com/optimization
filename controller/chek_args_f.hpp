#pragma once

#include <functional>
#include <type_traits>

// === ПОДСЧЕТ АРГУМЕНТОВ ДЛЯ ЛЮБОГО ВЫЗЫВАЕМОГО ОБЪЕКТА ===
template<typename Func>
struct function_counter;

// Для обычных функций
template<typename Ret, typename... Args>
struct function_counter<Ret(Args...)> {
    static constexpr int value = sizeof...(Args);
};

// Для указателей на функции
template<typename Ret, typename... Args>
struct function_counter<Ret(*)(Args...)> {
    static constexpr int value = sizeof...(Args);
};

// Для std::function (ГЛАВНОЕ)
template<typename Ret, typename... Args>
struct function_counter<std::function<Ret(Args...)>> {
    static constexpr int value = sizeof...(Args);
};

// Для const std::function
template<typename Ret, typename... Args>
struct function_counter<const std::function<Ret(Args...)>> {
    static constexpr int value = sizeof...(Args);
};

// Для ссылок на std::function
template<typename Ret, typename... Args>
struct function_counter<std::function<Ret(Args...)>&> {
    static constexpr int value = sizeof...(Args);
};

template<typename Ret, typename... Args>
struct function_counter<const std::function<Ret(Args...)>&> {
    static constexpr int value = sizeof...(Args);
};

// Для лямбд (автоматически)
template<typename Func>
struct function_counter {
    static constexpr int value = function_counter<decltype(&Func::operator())>::value;
};

// === УДОБНЫЕ ФУНКЦИИ ===
template<typename Func>
constexpr int count_std_functions(Func&&) {
    using Decayed = std::decay_t<Func>;
    return function_counter<Decayed>::value;
}

template<typename Func>
constexpr int count_std_functions_type() {
    return function_counter<std::decay_t<Func>>::value;
}
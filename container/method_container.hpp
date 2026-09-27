#pragma once
#define _USE_MATH_DEFINES 
#include <cmath>
#include <string>

#include <type_traits>
#include <vector>

template<typename T, typename = void>
struct scalar_of {
    using type = T;                     // не контейнер — оставляем как есть
};

template<typename T>
struct scalar_of<T, std::void_t<typename T::value_type>> {
    using type = typename T::value_type;
};

template<typename T>
using scalar_of_t = typename scalar_of<T>::type;

template<typename T>
struct is_vector : std::false_type {};

template<typename T, typename Alloc>
struct is_vector<std::vector<T, Alloc>> : std::true_type {};

// Проверка на vector
template<typename T>
inline constexpr bool is_vector_v = is_vector<T>::value;

// ============ ОПРЕДЕЛЕНИЕ ВСЕХ КОНСТАНТ ============
#define CONSTANTS_LIST \
    X(EPS, 1e-6, false) \
    X(TAU, ((std::sqrt(5.0) - 1.0) / 2.0), true) \
    X(E, M_E, true)\
    X(H,static_cast<scalar_of_t<T>>(0.01), false)\
    X(B, static_cast<scalar_of_t<T>>(0.9), false)

// ============ ЕДИНЫЙ МАКРОС ДЛЯ ГЕНЕРАЦИИ ============
#define GENERATE_CONSTANT(name, value, is_const) \
    template<typename T> \
    class name##Container { \
    protected: \
        static inline scalar_of_t<T> name = static_cast<scalar_of_t<T>>(value); \
    public: \
        static scalar_of_t<T> get##name() { \
            return name; \
        } \
        /* Добавляем set только если is_const == false */ \
        template<bool Enable = is_const> \
        static typename std::enable_if<!Enable, void>::type \
        set##name(scalar_of_t<T> val) { \
            name = val; \
        } \
    };

// ============ ГЕНЕРАЦИЯ ENUM ============
#define X(name, value, is_const) name,
enum class ConstantName {
    CONSTANTS_LIST
    COUNT
};
#undef X

// ============ ГЕНЕРАЦИЯ ВСЕХ КЛАССОВ ============
#define X(name, value, is_const) GENERATE_CONSTANT(name, value, is_const)
CONSTANTS_LIST
#undef X
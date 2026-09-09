#pragma once
#define _USE_MATH_DEFINES 
#include <cmath>
#include <string>

// ============ ОПРЕДЕЛЕНИЕ ВСЕХ КОНСТАНТ ============
#define CONSTANTS_LIST \
    X(EPS, 1e-6, false) \
    X(TAU, ((std::sqrt(5.0) - 1.0) / 2.0), true) \
    X(E, M_E, true)

// ============ ЕДИНЫЙ МАКРОС ДЛЯ ГЕНЕРАЦИИ ============
#define GENERATE_CONSTANT(name, value, is_const) \
    template<typename T> \
    class name##Container { \
    protected: \
        static inline T name = static_cast<T>(value); \
    public: \
        static T get##name() { \
            return name; \
        } \
        /* Добавляем set только если is_const == false */ \
        template<bool Enable = is_const> \
        static typename std::enable_if<!Enable, void>::type \
        set##name(T val) { \
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
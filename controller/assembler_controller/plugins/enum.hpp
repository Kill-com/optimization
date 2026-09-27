#pragma once
#include <iostream>
#include <unordered_map>
#include <string>

#define IF_VECTOR is_vector_v<T>

// Если T — скаляр (не вектор)
#define IF_SCALAR !is_vector_v<T>

// ГРУППЫ ПЛАГИНОВ (добавлять новые сюда)
#define PLUGINS_METHODS \
    X(GOLD_SECH, IF_VECTOR,1)  \
    X(PORABOLA, IF_VECTOR,1)\
    X(GRADNESTEROV, IF_VECTOR,4)

#define PLUGINS_FUNCTIONS \
    X(F_LIST,IF_SCALAR,0)\
    X(INTRESTIN_F,IF_VECTOR,0)

#define PLUGINS_BRACKETS \
    X(BRACKETGEOM,IF_VECTOR,0)\
    X(BRACKETTAU,IF_VECTOR,0)\
    X(BRACKETTAU2,IF_VECTOR,0)\
    X(BRACKETTAU3,IF_VECTOR,0)\
    X(BRACKETCONSTANT,IF_VECTOR,0)
    
#define PLUGINS_CALCS \
    X(SECONDSTEP,IF_VECTOR,0)\
    X(FIRSTSTEP,IF_VECTOR,0)

#define PLUGINS_HELPERS \
    PLUGINS_BRACKETS\
    PLUGINS_CALCS

// ОБЪЕДИНЕННЫЙ ENUM ДЛЯ ВСЕХ ПЛАГИНОВ
#define PLUGINS_ALL \
    PLUGINS_METHODS \
    PLUGINS_FUNCTIONS\
    PLUGINS_HELPERS

#define X(name, m,i) name,
// ОТДЕЛЬНЫЙ ENUM ДЛЯ МЕТОДОВ
enum class PLUGINS_METHOD {
    PLUGINS_METHODS
};

// ОТДЕЛЬНЫЙ ENUM ДЛЯ ФУНКЦИЙ
enum class PLUGINS_FUNCTION {
    PLUGINS_FUNCTIONS
};
enum class PLUGINS_BRACKET {
    PLUGINS_BRACKETS
};
enum class PLUGINS_CALC {
    PLUGINS_CALCS
};

enum class  PLUGINS {
    PLUGINS_ALL
};
#undef X
// Или получить только имена методов
inline std::vector<std::string> methodNames = {
    #define X(name,m,i) #name,
    PLUGINS_METHODS
    #undef X
};

inline std::vector<std::string> functionNames = {
    #define X(name,m,i) #name,
    PLUGINS_FUNCTIONS
    #undef X
};
// MAP ДЛЯ ПОИСКА ПО ИМЕНИ
inline std::unordered_map<std::string, int> PluginsMap = {
    #define X(name,m,i) {#name, static_cast<int>(PLUGINS::name)},
    PLUGINS_ALL
    #undef X
};

constexpr int funcs_for(PLUGINS p) {
    switch (p) {
        #define X(name,m,i)\
        case PLUGINS::name: return i;
        PLUGINS_ALL
        #undef X
        default: return 0;
    }
}
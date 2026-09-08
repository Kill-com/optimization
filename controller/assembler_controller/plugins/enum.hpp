#pragma once
#include <iostream>
#include <unordered_map>
#include <string>

// ГРУППЫ ПЛАГИНОВ (добавлять новые сюда)
#define PLUGINS_METHODS \
    X(GOLD_SECH)  \
    X(PORABOLA)

#define PLUGINS_FUNCTIONS \
    X(F_LIST)

// ОТДЕЛЬНЫЙ ENUM ДЛЯ МЕТОДОВ
enum class PLUGINS_METHOD {
    #define X(name) name,
    PLUGINS_METHODS
    #undef X
};

// ОТДЕЛЬНЫЙ ENUM ДЛЯ ФУНКЦИЙ
enum class PLUGINS_FUNCTION {
    #define X(name) name,
    PLUGINS_FUNCTIONS
    #undef X
};

// ОБЪЕДИНЕННЫЙ ENUM ДЛЯ ВСЕХ ПЛАГИНОВ
#define PLUGINS_ALL \
    PLUGINS_METHODS \
    PLUGINS_FUNCTIONS

enum class  PLUGINS {
    #define X(name) name,
    PLUGINS_ALL
    #undef X
};

// Или получить только имена методов
inline std::vector<std::string> methodNames = {
    #define X(name) #name,
    PLUGINS_METHODS
    #undef X
};

inline std::vector<std::string> functionNames = {
    #define X(name) #name,
    PLUGINS_FUNCTIONS
    #undef X
};
// MAP ДЛЯ ПОИСКА ПО ИМЕНИ
inline std::unordered_map<std::string, int> PluginsMap = {
    #define X(name) {#name, static_cast<int>(PLUGINS::name)},
    PLUGINS_ALL
    #undef X
};
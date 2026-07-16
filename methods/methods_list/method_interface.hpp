#pragma once
#include <variant>
#include <vector>
#include <string>

// Тип данных, который может быть int или vector<int>
using PluginArg = std::variant<float, std::vector<float>>;

// Тип результата
using PluginResult = std::variant<int, std::vector<int>>;

// Интерфейс плагина
extern "C" {
    PluginResult f(const PluginArg& arg);
}
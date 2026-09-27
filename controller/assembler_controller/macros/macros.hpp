#pragma once

#include <variant>

#define REGISTER_FUNCTION \
public:\
static auto f_() { \
    return &f; \
}\
private:

#define REPACK(name_function,output,...)\
    return std::visit([&](auto&& name_function##_) -> output{\
        __VA_ARGS__\
    }, name_function);



#pragma once

#include <variant>

#define REGISTER_FUNCTION \
static auto f_() { \
    return &f; \
}

#define REPACK(name_function,...)\
    return std::visit([&](auto&& name_function##_) -> T{\
        __VA_ARGS__\
    }, name_function);



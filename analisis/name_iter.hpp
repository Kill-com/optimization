#pragma once
#include <string>

enum class Iter {
    FOR_COUNT,
    WHILE_COUNT
};
template<auto name>
std::string how_iter() {
    switch (name) {
        case Iter::FOR_COUNT:
            return "Using for loop: ";
        case Iter::WHILE_COUNT:
            return "Using while loop: ";
        default:
            return "Unknown iteration type";
    }
}
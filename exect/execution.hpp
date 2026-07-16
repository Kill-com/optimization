#pragma once

#include <iostream>
#include <string_view>
//запуск методов на решение целевой функции

class METHOD_L{
    protected:
    std::string_view methods;
    public:
    METHOD_L(std::string_view m=""):methods(m){};
    virtual void exect(std::string_view);
    virtual ~METHOD_L() = default;
};

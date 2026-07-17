#pragma once

#include <iostream>
//запуск методов на решение целевой функции

class METHOD_L{
    protected:
    std::string methods;
    public:
    METHOD_L(std::string m=""):methods(m){};
    virtual void exect(std::string);
    virtual ~METHOD_L() = default;
};

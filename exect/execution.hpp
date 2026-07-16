#pragma once

#include <iostream>
#include <vector>
#include <string_view>
//запуск методов на решение целевой функции

class METHOD_L{
    protected:
    std::string_view methods;
    std::string_view functions;
    public:
    METHOD_L(std::string_view m="", std::string_view f=""):methods(m), functions(f){};
    virtual void exect()=0;
    virtual ~METHOD_L() = default;
};
class EXECUTER: protected METHOD_L{
    public:
    EXECUTER(std::string_view m="", std::string_view f=""):METHOD_L(m,f){};
    void exect();
};

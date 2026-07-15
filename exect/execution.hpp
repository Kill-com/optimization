#pragma once

#include <iostream>
#include <vector>
#include <string_view>
//запуск методов на решение целевой функции

class METHOD_L{
    protected:
    int count;
    std::string_view methods;
    public:
    METHOD_L(int c=0, std::string_view m=NULL):count(c),methods(m){};
    virtual void exect(std::string_view method, std::string_view function)=0;
    virtual ~METHOD_L() = default;
};
class EXECUTER: protected METHOD_L{
    public:
    void exect(std::string_view method, std::string_view function);
};

#pragma once //защита от множественного запуска

#include <iostream>
#include <vector>
#include <string_view>
//запуск методов на решение целевой функции

class METHOD_L{ // Базовый абстрактный класс методов решения функции
    protected:
    std::string_view methods; //название метода
    std::string_view functions; //название фукнции
    public:
    METHOD_L(std::string_view m="", std::string_view f=""):methods(m), functions(f){};
    virtual void exect()=0;
    virtual ~METHOD_L() = default;
};
class EXECUTER: protected METHOD_L{ //класс, запускающий конкретный метод
    public:
    EXECUTER(std::string_view m="", std::string_view f=""):METHOD_L(m,f){}; //передачв названия метода и функции в базовый класс
    void exect();
};

#pragma once

#include <functional>

//интерфейс для методов
template <typename T>
class method
{
private:
    //параметры. Могут быть дополнены в производных классах по необходимости
    T eps = static_cast<T>(1e-6);
    int maxIterations = int{};
public:
    //можно менять параметры
    void setEpsilon(T e) override { eps = e; }
    void setMaxIterations(int maxIter) override { maxIterations = maxIter; }

    virtual ~method() = default;

    //реализация метода
    virtual T optimize(std::function<T(T)> func) const = 0;

};



template<typename T>
class IMethod{
public:
};
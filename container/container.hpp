#pragma once

#include <string>
#include <vector>

#include "controller_container.hpp"


//базовый шаблонный класс для хранения любого объекта T. Основа контейнеров
template<typename T>
class Icontainer{
private:
    T info;
public:
//конструктор с переменным числом аргументов
    template<typename ...Args>
    Icontainer(Args&&...args):info(std::forward<Args>(args)...){};
//преобразование к типу Т
    operator T(){return info;};
//получение НЕконстантной ссылки на хранимое значение
    T& getinfo(){return info;};
//получение КОНСТАНТНОЙ ссылки на хранимое значение
    const T& getinfo() const { return info; }
//метод изменяющий хранимое значение
    void inputinfo(T&& i){info=i;};
};

//класс, хранящий вектор элементов типа Т
//наследует базовые методы контейнера и методы работы с векторами
template<typename T>
class IcontainerVector : public Icontainer<std::vector<T>>,public ControlVector<IcontainerVector,T> {
public:
    IcontainerVector(std::vector<T> info)
        : Icontainer<std::vector<T>>(std::move(info)) {}

    // Для списка элементов
    IcontainerVector(std::initializer_list<T> init)
        : Icontainer<std::vector<T>>(std::vector<T>(init)) {}

    // Для одного аргумента
    template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
    IcontainerVector(U&& u)
        : Icontainer<std::vector<T>>(std::vector<T>{std::forward<U>(u)}) {}
};

template<auto Name>
class ContainerCount: public Icontainer<int>, public ControlCount<ContainerCount<Name>, Name>{
public:
    ContainerCount() : Icontainer<int>(0) {};
};

//класс для хранения строки
class containerStr:public Icontainer<std::string>{
public:
    using Icontainer<std::string>::Icontainer;
};

//класс для хранения вектора строк
class containerVectorStr:public IcontainerVector<std::string>{
public:
    using IcontainerVector<std::string>::IcontainerVector;
};

//шаблонный класс для хранения кортежа
template<typename ...Args>
class ContainerValue:public Icontainer<std::tuple<Args...>>,
    public ControlTuple{
public:
    using Icontainer<std::tuple<Args...>>::Icontainer;
};

// Универсальный шаблонный класс-контейнер для хранения значения любого типа T
//можно использовать для создания именованных типов контейнеров
template<typename T>
class containerClass: public Icontainer<T>{
public:
    using Icontainer<T>::Icontainer;
};

#pragma once

#include <string>
#include <vector>

#include "controller_container.hpp"

template<typename T>
class Icontainer{
private:
    T info;
public:
    template<typename ...Args>
    Icontainer(Args&&...args):info(std::forward<Args>(args)...){};
    operator T(){return info;};
    T& getinfo(){return info;};
    const T& getinfo() const { return info; }
    void inputinfo(T&& i){info=i;};
};
template<typename T>
class IcontainerVector:public Icontainer<std::vector<T>>,
    public ControlVector<IcontainerVector,T>{
public:
    IcontainerVector(std::vector<T> info):
    Icontainer<std::vector<T>>(info){};
    template<typename ...Args>
    IcontainerVector(Args&&...args):Icontainer<std::vector<T>>(std::forward<Args>(args)...){};
};


class containerStr:public Icontainer<std::string>{
public:
    using Icontainer<std::string>::Icontainer;
};
class containerVectorStr:public IcontainerVector<std::string>{
public:
    using IcontainerVector<std::string>::IcontainerVector;
};
template<typename ...Args>
class ContainerValue:public Icontainer<std::tuple<Args...>>,
    public ConrolTuple{
public:
    using Icontainer<std::tuple<Args...>>::Icontainer;
};
template<typename T>
class containerClass: public Icontainer<T>{
public:
    using Icontainer<T>::Icontainer;
};

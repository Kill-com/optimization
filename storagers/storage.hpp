#pragma once

#include <string>
#include <vector>

#include "controler_storage.hpp"

template<typename T>
class IStorage{
private:
    T info;
public:
    template<typename ...Args>
    IStorage(Args&&...args):info(std::forward<Args>(args)...){};
    operator T(){return info;};
    T& getinfo(){return info;};
    const T& getinfo() const { return info; }
    void inputinfo(T&& i){info=i;};
};
template<typename T>
class IStorageVector:public IStorage<std::vector<T>>,
    public ControlVector<IStorageVector,T>{
public:
    IStorageVector(std::vector<T> info):
    IStorage<std::vector<T>>(info){};
    template<typename ...Args>
    IStorageVector(Args&&...args):IStorage<std::vector<T>>(std::forward<Args>(args)...){};
};


class StorageStr:public IStorage<std::string>{
public:
    using IStorage<std::string>::IStorage;
};
class StorageVectorStr:public IStorageVector<std::string>{
public:
    using IStorageVector<std::string>::IStorageVector;
};
template<typename ...Args>
class StorageValue:public IStorage<std::tuple<Args...>>,
    public ConrolTuple{
public:
    using IStorage<std::tuple<Args...>>::IStorage;
};
template<typename T>
class StorageClass: public IStorage<T>{
public:
    using IStorage<T>::IStorage;
};

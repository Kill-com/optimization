#pragma once

#include "../../storagers/storage.hpp"
#include "../assembler_control/plugins_control.hpp"

class ICollectStart{
protected:
    StorageStr method;
    StorageVectorStr target_function;
public:
    template<typename ...Args>
    ICollectStart(std::string method_name, Args&&... function_name):
    method(StorageStr(method_name)),
    target_function(StorageVectorStr(std::forward<Args>(function_name)...))
    {}
};

class IStartCalculation{
    public:
    virtual void operator()()=0;
    virtual ~IStartCalculation()=default;
};

template<typename ...Args>
class IExect{
private:
    auto getwrapped_start(){
        return [](auto&& method, auto&&...args){
            if constexpr (std::is_invocable_v<decltype(method), decltype(args)...>){
                return method(std::forward<decltype(args)>(args)...);
            }
        };
    }
protected:
    StorageValue<Args...> value;
    template<typename... Plugins, typename Tuple>
    void unpacking_tuple(Tuple&& tuple,Plugins&&...func){
        auto all_args = std::tuple_cat(
            std::tuple(std::forward<Plugins>(func))...,
            std::forward<Tuple>(tuple)
        );
        std::apply(getwrapped_start(), all_args);
    }
public:
    IExect(StorageValue<Args...>& vl):value(vl){};
};
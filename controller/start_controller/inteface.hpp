#pragma once

#include "../../container/container.hpp"
#include "../assembler_controller/plugins_controller.hpp"

class ICollectStart{
protected:
    containerStr method;
    containerVectorStr target_function;
public:
    template<typename ...Args>
    ICollectStart(std::string method_name, Args&&... function_name):
    method(containerStr(method_name)),
    target_function(containerVectorStr(std::forward<Args>(function_name)...))
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
    containerValue<Args...> value;
    template<typename... Plugins, typename Tuple>
    void unpacking_tuple(Tuple&& tuple,Plugins&&...func){
        auto all_args = std::tuple_cat(
            std::tuple(std::forward<Plugins>(func))...,
            std::forward<Tuple>(tuple)
        );
        std::apply(getwrapped_start(), all_args);
    }
public:
    IExect(containerValue<Args...>& vl):value(vl){};
};
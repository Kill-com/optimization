#pragma once

#include <functional>
#include <thread>

#include "../macros/macros.hpp"


#include "../../../analisis/macros.hpp"
#include "../../../container/method_container.hpp"
#include "../../controller_thread/Ithread.hpp"

#define USING_ALL(...)\
    using Base = IMethod<__VA_ARGS__>;\
    using Base::logger;\
    using typename Base::type_var_foo;\
    using typename Base::type_SelfWrite_foo;\
    using typename Base::type_normal_foo;\
    using Base::get_result_foo;
#define LOG_METHOD(name) \
    logger->template method_log<T>(name);

template<typename T>
class SimpleSearch:public EPSContainer<T>,
    public TAUContainer<T>,
    public EContainer<T>{
protected:
    using EPSContainer<T>::EPS;
    using TAUContainer<T>::TAU;
    using EContainer<T>::E;
};

template<typename Output, typename Input=Output>
class IMethod:public ThreadLOG{
private:
    template<typename... Args>
    static auto make_map(Args... args) {
        std::unordered_map<std::string, std::vector<Input>> map;
        int i = 1;
        ((map["x" + std::to_string(i++)]=
            std::vector<Input>{ static_cast<Input>(args) }), ...);
        return map;
    }
protected:
    using ThreadLOG::logger;
    using type_SelfWrite_foo =
    std::function<
    std::vector<Output>
    (const std::unordered_map<std::string, std::vector<Input>>&)>;

    using type_normal_foo= std::function<Output(Input)>;

    using type_var_foo= std::variant<type_SelfWrite_foo, type_normal_foo>;

    template<typename ...Args>
    static Output get_result_foo(const type_normal_foo& name, Args&& ...args){
        return name(std::forward<Args>(args)...);
    }

    template<typename A>
    static Output get_result_foo(const type_SelfWrite_foo& name, A&& a){
        return name(make_map(a))[0];
    }
    template<typename A, typename ...Args>
    static std::vector<Output> get_result_foo(const type_SelfWrite_foo& name,A&& a, Args&& ...args){
        return name(make_map(std::forward<A>(a), std::forward<Args>(args)...));
    }
};

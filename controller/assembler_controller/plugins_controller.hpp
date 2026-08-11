#pragma once
#include <iostream>
#include <string>

#include "../chek_args_f.hpp"
#include "../../container/container.hpp"


class CollectPlug{
protected:
    template<typename T,typename Func>
    void collect(Func, const std::string);

    // template<typename T,typename Func, typename Func_str>
    // void collect(Func func, Func_str plugin){
    //     func(plugin);
    // }
};

template<typename T>
class AssemblerSimple: public CollectPlug{
private:
    T container_class;
    containerVectorStr container_func;
    size_t count;
    template<typename TypeArg, size_t count_,typename PluginProcces, typename... Funcs_Assembling>
    void compiled_simple_impl(PluginProcces&& process, 
                              size_t idx,
                              Funcs_Assembling&&... funcs) {
        if (idx >= count) return;
        if constexpr(count_>0){
            if(idx>0){
                auto wrapper = [this, process, idx, &funcs...](auto&& wrapped_args) {
                    // Рекурсивный вызов со следующим индексом
                    this->compiled_simple_impl<TypeArg,count_-1>(
                        process,
                        idx - 1,
                        std::forward<decltype(wrapped_args)>(wrapped_args),
                        std::forward<Funcs_Assembling>(funcs)...
                    );
                };
                collect<TypeArg>(wrapper, container_func[idx]);
                return;
            }
            if(idx==0){
                auto wrapper = [this, process, idx, &funcs...](auto&& wrapped_args){
                    // Финальный вызов
                    this->container_class.exect(process,
                        std::forward<decltype(wrapped_args)>(wrapped_args),
                        std::forward<Funcs_Assembling>(funcs)...
                    );
                };
                collect<TypeArg>(wrapper, container_func[idx]);
            }
        }
    }   
protected:
    template<typename TypeArg, typename PluginProcces>
    void compiled_simple(PluginProcces&& process) {
        if (count > 0) {
            // Начинаем с последнего индекса (count - 1)
            compiled_simple_impl<
            TypeArg,
            function_counter<std::decay_t<PluginProcces>>::value
            >(
                std::forward<PluginProcces>(process),
                count - 1
            );
        }
    }

public:
    AssemblerSimple(const T& t,const containerVectorStr& container):
    container_class(t), container_func(container){
        count=container_func.getsize();
    };
};

class AssemblerComplex: public CollectPlug{
protected:
    template<typename TypeArg,typename Next>
    void compiled_complex(Next next, std::string name){
        auto wrapper = [&next](auto&& process) {
            next(std::forward<decltype(process)>(process));
        };
        collect<TypeArg>(wrapper, name);
    }
};

template<typename T>
class StartPlug: public AssemblerSimple<T>, public AssemblerComplex{
public:
    using AssemblerSimple<T>::AssemblerSimple;
    template<typename TypeArg>
    void start_plug(std::string name_method){
        auto wrapper = [this](auto&& method) {
            this->template compiled_simple<TypeArg>(
                std::forward<decltype(method)>(method)
            );
        };
        this->compiled_complex<TypeArg>(wrapper,name_method);
    }
};

#include "plugins.tpp"


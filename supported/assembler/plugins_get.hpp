#pragma once
#include <string>
#include "../check_arg_f.hpp"
#include "../control_args.hpp"
#include "../tuple_control.hpp"

class StorageFunc{
public:
    template<typename Methods, typename... Args>
    void start(Methods m, Args&&... args){
        if constexpr (std::is_invocable_v<Methods, Args...>){
            m(std::forward<Args>(args)...);
        }
    }
};

class CollectPlug{
    protected:
    template<typename Func, typename... Args>
    void collect(Func, const std::string&, Args&&...);

    template<typename Func, typename Func_str, typename... Args>
    void collect(Func func, Func_str plugin, Args&&... args){
        func(plugin,std::forward<Args>(args)...);
    }
};

template<typename Derived>
class Assembler:public CollectPlug{
private:
    int count=0;
    size_t count_=0;

protected:
    template<typename Function, typename... Args>
    void check_count_arg_func(Function func,Args&&... args){
        count=count_std_functions(func);
        count_=count_std_functions(func);
        compiled_simple(func,std::forward<Args>(args)...);
    }

    template<typename PluginProcces, typename Func_str, typename... Args>
    void compiled_simple(PluginProcces&& process, Func_str&& func_name, Args&&... args){
        auto wrapper = [process, this](auto&& target_f, auto&&... wrapped_args) {
            this->count--;
            if (this->count == 0) {
                auto& derived = static_cast<Derived&>(*this);
                derived.start(process,
                    std::forward<decltype(target_f)>(target_f),
                    std::forward<decltype(wrapped_args)>(wrapped_args)...
                );
            }// else if(this->count>0){
                
            //     // Создаем кортеж из аргументов, распаковывая optional
            //     auto args_tuple = std::tuple_cat(
            //         AssemArg::to_tuple(std::forward<decltype(wrapped_args)>(wrapped_args))...
            //     );
                
            //     // Создаем менеджер из распакованных аргументов
            //     auto manager = std::apply(
            //         [](auto&&... elems) {
            //             return MakeClasses::makeclass<ManagementArgs>(
            //                 std::forward<decltype(elems)>(elems)...
            //             );
            //         },
            //         args_tuple
            //     );
                
            //     auto first = manager.getfirst();
            //     auto second = manager.getfor_i(this->count_ - 2);
            //     auto third = manager.getnotfor_i(this->count_ - 2);
                
            //     // Собираем все в кортеж
            //     auto end = std::tuple_cat(
            //         AssemArg::to_tuple(process),
            //         AssemArg::to_tuple(first),
            //         AssemArg::to_tuple(second),
            //         AssemArg::to_tuple(std::forward<decltype(target_f)>(target_f)),
            //         AssemArg::to_tuple(third)
            //     );
            //     // Распаковываем и вызываем
            //     std::apply(
            //         [&](auto&&... unpacked) {
            //             this->compiled_simple(
            //                 std::forward<decltype(unpacked)>(unpacked)...
            //             );
            //         },
            //         end
            //     );
            // }
        };
        collect(wrapper, func_name, std::forward<Args>(args)...);
    }

    template<typename Next, typename... Args>
    void compiled_complex(Next next, std::string name, Args&&... args){
        auto wrapper = [&next](auto&& process, auto&&... wrapped_args) {
            next(std::forward<decltype(process)>(process),
                std::forward<decltype(wrapped_args)>(wrapped_args)...
            );
        };
        collect(wrapper, name, std::forward<Args>(args)...);
    }
};


class StartPlug: public StorageFunc, public Assembler<StartPlug>{
    public:
    template<typename... Args>
    void operator()(std::string name_method, Args&&... args){
        auto wrapper = [this](auto&& func, auto&&... wrapped_args) {
            this->check_count_arg_func(
                std::forward<decltype(func)>(func),
                std::forward<decltype(wrapped_args)>(wrapped_args)...
            );
        };
        compiled_complex(wrapper,name_method,std::forward<Args>(args)...);
    }
};

#include "plugins.tpp"
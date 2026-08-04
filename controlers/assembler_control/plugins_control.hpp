#pragma once
#include <iostream>
#include <string>

#include "../chek_args_f.hpp"
#include "../../storagers/storage.hpp"


class CollectPlug{
    protected:
    template<typename Func>
    void collect(Func, const std::string);

    template<typename Func, typename Func_str>
    void collect(Func func, Func_str plugin){
        func(plugin);
    }
};

template<typename T>
class AssemblerSimple: public CollectPlug{
private:
    T storage_class;
    StorageVectorStr storage_func;
    size_t count;
    template<size_t count_,typename PluginProcces, typename... Funcs_Assembling>
    void compiled_simple_impl(PluginProcces&& process, 
                              size_t idx,
                              Funcs_Assembling&&... funcs) {
        // static_assert(sizeof...(Funcs_Assembling) < 100, "Too deep recursion!");
        if (idx >= count) return;
        if constexpr(count_>0){
            if(idx>0){
                auto wrapper = [this, process, idx, &funcs...](auto&& wrapped_args) {
                    // Рекурсивный вызов со следующим индексом
                    this->compiled_simple_impl<count_-1>(
                        process,
                        idx - 1,
                        std::forward<decltype(wrapped_args)>(wrapped_args),
                        std::forward<Funcs_Assembling>(funcs)...
                    );
                };
                collect(wrapper, storage_func[idx]);
                return;
            }
            if(idx==0){
                auto wrapper = [this, process, idx, &funcs...](auto&& wrapped_args){
                    // Финальный вызов
                    this->storage_class(process,
                        std::forward<decltype(wrapped_args)>(wrapped_args),
                        std::forward<Funcs_Assembling>(funcs)...
                    );
                };
                collect(wrapper, storage_func[idx]);
            }
        }
    }   
protected:
    template<typename PluginProcces>
    void compiled_simple(PluginProcces&& process) {
        if (count > 0) {
            // Начинаем с последнего индекса (count - 1)
            compiled_simple_impl<count_std_functions(process)>(
                std::forward<PluginProcces>(process),
                count - 1
            );
        }
    }
    // template<typename PluginProcces,typename... Funcs_Assembling>
    // void compiled_simple(PluginProcces&& process,Funcs_Assembling&&... funcs){
    //     static size_t current_index = count;
    //     auto wrapper = [process, this](auto&&... wrapped_args) {
    //         static size_t idx = current_index - 1;
    //         if (current_index <= 0) {
    //             this->storage_class(process,
    //                 std::forward<decltype(wrapped_args)>(wrapped_args)...
    //             );
    //         }else{
    //             this->compiled_simple(
    //                 process,
    //                 std::forward<decltype(wrapped_args)>(wrapped_args)...
    //             );
    //         }
    //     };
    //     count--;
    //     collect(wrapper, storage_func[count], std::forward<Funcs_Assembling>(funcs)...);
    // }
public:
    AssemblerSimple(const T& t,const StorageVectorStr& storage):
    storage_class(t), storage_func(storage){
        count=storage_func.getsize();
    };
};

class AssemblerComplex: public CollectPlug{
protected:
    template<typename Next>
    void compiled_complex(Next next, std::string name){
        auto wrapper = [&next](auto&& process) {
            next(std::forward<decltype(process)>(process));
        };
        collect(wrapper, name);
    }
};

template<typename T>
class StartPlug: public AssemblerSimple<T>, public AssemblerComplex{
public:
    using AssemblerSimple<T>::AssemblerSimple;
    void operator()(std::string name_method){
        auto wrapper = [this](auto&& method) {
            this->compiled_simple(
                std::forward<decltype(method)>(method)
            );
        };
        this->compiled_complex(wrapper,name_method);
    }
};

#include "plugins.tpp"


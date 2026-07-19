#pragma once

#include <iostream>

#include "../supported/assembler/assembling_methods.hpp"
//запуск методов на решение целевой функции

class METHOD_L{
    protected:
    std::string methods;
    public:
    METHOD_L(std::string m=""):methods(m){};

    template<typename Func,typename... Args>
    void collect(Func func, const std::string functions, Args&&... args){
        auto method = PluginLoader::loadLibrary(this->methods);
        auto function = PluginLoader::loadLibrary(functions);
        if (!method || !function) {
            std::cerr << "Failed to load plugin!" << std::endl;
        }else{
            using ProcessFunc_m = float(std::function<float(float)>, float, float);
            auto* process = PluginLoader::getFunction<ProcessFunc_m>(method.get(), "f");
            using ProcessFunc_f = float(float);
            auto process_f = PluginLoader::getFunction<ProcessFunc_f>(function.get(), "target_f");
            if (!process || !process_f) {
                std::cerr << "Function 'f' not found!" << std::endl;
            }else{
                func(process, process_f, std::forward<Args>(args)...);
            }
        }
    }

    template<typename... Args>
    void operator()(const std::string& functions,Args&&... args){
        auto exect_wrapper = [this](auto process, auto process_f, auto&&... wrapped_args) {
            this->exect(process, process_f, std::forward<decltype(wrapped_args)>(wrapped_args)...);
        };
        
        collect(exect_wrapper, functions, std::forward<Args>(args)...);
    }
    template<typename Methods, typename Functions, typename... Args>
    void exect(Methods m, Functions f, Args&&... args){
        m(f, std::forward<Args>(args)...);
    }
    virtual ~METHOD_L() = default;
};

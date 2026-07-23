#pragma once

#include <iostream>

#include "../supported/assembler/assembling_methods.hpp"
//запуск методов на решение целевой функции

class Conteiner{
    protected:
    std::string methods;
    Conteiner(std::string m=""):methods(m){};
};

class Collected_sig{
    protected:
    using ProcessFunc_f=float(float);
    using ProcessFunc_m = float(std::function<float(float)>, float, float);
};

class Collected:private Collected_sig{
    private:
    LibraryPtr load(const std::string func_path){
        LibraryPtr func = PluginLoader::loadLibrary(func_path);
        if (!func) {
            std::cerr << "Failed to load plugin!" << std::endl;
            return NULL;
        }else{
            return func;
        }
    }
    protected:
    template<typename Func,typename... Args>
    void collecting(Func func ,const std::string methods_ ,const std::string functions_, Args&&... args){
        auto method = load(methods_);
        auto function = load(functions_);
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
    // Сборка для функции пользователя

    template<typename Func,typename Functios,typename... Args>
    void collecting(Func func,const std::string methods_, Functios functions, Args&&... args){
        auto method = load(methods_);
        using ProcessFunc_m = float(std::function<float(float)>, float, float);
        auto* process = PluginLoader::getFunction<ProcessFunc_m>(method.get(), "f");
        if (!process) {
            std::cerr << "Function 'f' not found!" << std::endl;
        }else{
            func(process, functions, std::forward<Args>(args)...);
        }
    }
};

class METHOD_L: public Conteiner, protected Collected{
    public:
    METHOD_L(std::string m=""):Conteiner(m){};
    template<typename... Args>
    void operator()(const std::string& functions,Args&&... args){
        auto exect_wrapper = [this](auto process, auto process_f, auto&&... wrapped_args) {
            this->exect(process, process_f, std::forward<decltype(wrapped_args)>(wrapped_args)...);
        };
        
        collecting(exect_wrapper, methods, functions, std::forward<Args>(args)...);
    }
    template<typename Func,typename... Args>
    void operator()(Func functions,Args&&... args){
        auto exect_wrapper = [this](auto process, auto process_f, auto&&... wrapped_args) {
            this->exect(process, process_f, std::forward<decltype(wrapped_args)>(wrapped_args)...);
        };
        
        collecting(exect_wrapper, methods, functions, std::forward<Args>(args)...);
    }
    template<typename Methods, typename Functions, typename... Args>
    void exect(Methods m, Functions f, Args&&... args){
        // std::cout<<f;
        m(f, std::forward<Args>(args)...);
    }
    virtual ~METHOD_L() = default;
};

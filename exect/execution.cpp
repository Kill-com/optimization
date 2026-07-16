#include <iostream>
#include <variant>
#include <vector>

#include "execution.hpp"
#include "../supported/assembler/assembling_methods.hpp"

void EXECUTER::exect(){
    auto method = PluginLoader::loadLibrary(this->methods);
    auto function = PluginLoader::loadLibrary(this->functions);
    if (!method && !function) {
        std::cerr << "Failed to load plugin!" << std::endl;
    }else{
          using ProcessFunc_m = float(std::function<float(float)>, float, float);
          auto* process = PluginLoader::getFunction<ProcessFunc_m>(method.get(), "f");
          using ProcessFunc_f = float(float);
          auto process_f = PluginLoader::getFunction<ProcessFunc_f>(function.get(), "target_f");
          if (!process && !process_f) {
            std::cerr << "Function 'f' not found!" << std::endl;
            }else{
                std::cout<<process(process_f,1,2)<<std::endl;
            }
    }
}
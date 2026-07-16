#include <iostream>
#include <filesystem>

#include "supported/parser/parser_flags.hpp"

#include "exect/execution.hpp"
#include "analisis/decorators.hpp"


namespace fs = std::filesystem;

//checking the file extension
bool hasExtension(const std::string_view& filename, const std::string& ext) {
    fs::path path(filename);
    return path.extension() == ext;
}
int main(int argc, char* argv[]) {
    Parser parser;
    
    if (!parser.parse(argc, argv)) {
        std::cerr << "Use the -f or -function key to enter --function files"
         <<"or a json file with file addresses."
         <<'\n'
         <<"Use the optional -m or --method key to enter method names for the"
         <<" target function, or a json file with method names."
         <<'\n'
         <<"You can view a list of available methods in the list_method.json file."
         <<'\n'
         <<"Use the -s key if you want to solve your problem without analysis."
         << std::endl;
        return 1;
    }
    
    // Fast access
    std::string_view function;
    for (size_t i = 0; i < parser.getKey1Count(); i++) {
        function=parser.getKey1Arg(i);
        if(hasExtension(function,".json")){
            //tomorrow
        }
        std::cout << "  " << parser.getKey1Arg(i) << std::endl;
    }
    std::string_view method;
    for (size_t i = 0; i < parser.getKey2Count(); i++) {
        method=parser.getKey2Arg(i);
        if(hasExtension(method,".json")){
            //tomorrow
        }
        std::cout << "  " << parser.getKey2Arg(i) << std::endl;
    }
    
    if(parser.getflag()){
        //тут будет цикл а пока...
        EXECUTER A(method, function);
        A.exect();
    }else{

    }


    return 0;
}
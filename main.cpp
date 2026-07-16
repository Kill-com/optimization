#include <iostream>
#include <filesystem>

#include "supported/parser/parser_flags.hpp"

#include "exect/execution.hpp"
#include "decorators/decorators.hpp"

namespace fs = std::filesystem;
//checking the file extension
bool hasExtension(const std::string_view& filename, const std::string& ext) {
    fs::path path(filename);
    return path.extension() == ext;
}

template<typename classes>
void exect_(Parser& parser, int m=2, std::string_view do_to="", classes A=classes()) {
        bool need_retry = false;
        std::vector<std::string_view> failed_args;
        
        // Цикл по аргументам
        for (size_t i = 0; i < parser.getKeyCount(m); i++) {
            std::string_view function = parser.getKeyArg(i, m);
            
            // УСЛОВИЕ: если нет расширения .dll
            if (!hasExtension(function, ".dll")) {
                failed_args.push_back(function);
                need_retry = true;
            }else{
                if(m==2){
                    A=classes(function);
                    exect_(parser, 1, function,A);
                }else{
                    A.exect(function);
                }
            }
        }
        // УСЛОВИЕ ДЛЯ РЕКУРСИВНОГО ВЫЗОВА
        if (need_retry) {   
            // Создаём новый парсер с исправленными аргументами
            Parser_file new_parser;
            std::tuple<std::vector<std::string_view>> args ={failed_args};
            new_parser.parse(args);
            //  ВЫЗЫВАЕМ СЕБЯ С НОВЫМ ПАРСЕРОМ
            exect_(new_parser,m,do_to, A);
        }
}

void exect(Parser_terminal& parser){
    if(parser.getflag()){
        //обычный вызов
        exect_(parser,2,"",METHOD_L());
    }else{
        //вызов с анализлом
        exect_(parser,2,"",EXECTED());
    }
}


int main(int argc, char* argv[]) {
    Parser_terminal parser;
    std::tuple<int, char**> args = {argc, argv};
    if (!parser.parse(args)) {
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
    exect(parser);
    return 0;
}
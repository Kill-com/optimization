#include <iostream>
#include <filesystem>

#include "supported/parser/parser_flags.hpp"
namespace fs = std::filesystem;

#include "exect/execution.hpp"
#include "analisis/Analisis.hpp"


template<typename classes>
void exect_(Parser_terminal& parser, int m=2, classes A=classes()) {
    bool need_retry = false;
    std::vector<std::string> failed_args;
    // Цикл по аргументам
    for (size_t i = 0; i < parser.getKeyCount(m); i++) {
        std::string function = parser.getKeyArg(i, m);
        // УСЛОВИЕ: если нет расширения .dll
        if (fs::is_directory(function)) {
            failed_args.push_back(function);
            parser.del_el(m,i);
            i--;
            need_retry = true;
        }else{
            if(m==2){
                classes A(function);
                std::cout<<"Обработка. Метод: "<<function;
                Parser_terminal p(parser);
                exect_(p, 1,A);
            }else{
                std::cout<<" Функция: "<<function<<std::endl;
                A(function, 1,2);
            }
        }
    }
    // УСЛОВИЕ ДЛЯ РЕКУРСИВНОГО ВЫЗОВА
    if (need_retry) {   
        // Создаём новый парсер с исправленными аргументами
        Parser_file new_parser(parser);
        
        std::tuple<int,std::vector<std::string>> args ={m, failed_args};
        new_parser.parse(args);
        //  ВЫЗЫВАЕМ СЕБЯ С НОВЫМ ПАРСЕРОМ
        exect_(new_parser,m, A);
    }
}

void exect(Parser_terminal& parser){
    if(parser.getflag()){
        //обычный вызов
        exect_(parser,2,METHOD_L());
    }else{
        //вызов с анализлом
        exect_(parser,2,EXECTED());
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
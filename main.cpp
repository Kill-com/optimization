
#include "controller/start_controller/execution.hpp"
#include "controller/input_controller/parsers.hpp"
#include "analisis/executions.hpp"


// template<typename Parser,typename Classes>
// void exect_(Parser& parser, int m=2, Classes A=classes()) {
//     bool need_retry = false;
//     std::vector<std::string> failed_args;
//     // Цикл по аргументам
//     for (size_t i = 0; i < parser.getKeyCount(m); i++) {
//         std::string function = parser.getKeyArg(i, m);
//         // УСЛОВИЕ: если нет расширения .dll
//         if (fs::is_directory(function)) {
//             failed_args.push_back(function);
//             parser.del_el(m,i);
//             i--;
//             need_retry = true;
//         }else{
//             if(m==2){
//                 classes A(function);
//                 std::cout<<"Обработка. Метод: "<<function;
//                 Parser_terminal p(parser);
//                 exect_(p, 1,A);
//             }else{
//                 std::cout<<" Функция: "<<function<<std::endl;
//                 A(function, 1,2);
//             }
//         }
//     }
//     // УСЛОВИЕ ДЛЯ РЕКУРСИВНОГО ВЫЗОВА
//     if (need_retry) {   
//         // Создаём новый парсер с исправленными аргументами
//         Parser_file new_parser(parser);
        
//         std::tuple<int,std::vector<std::string>> args ={m, failed_args};
//         new_parser.parse(args);
//         //  ВЫЗЫВАЕМ СЕБЯ С НОВЫМ ПАРСЕРОМ
//         exect_(new_parser,m, A);
//     }
// }


void exect(ParserTerminal& parser){
    for (size_t i = 0; i < parser.getKeyCount(1); i++) {
        std::string method=parser.getKeyArg(i,1);
        for(size_t j=0; j<parser.getKeyCount(2);j++){
            std::string function=parser.getKeyArg(j,2);
            std::initializer_list<std::string> args = {function};
            if(parser.get_for_analis()){
                StartCalculation<SimpleExect,int,int> st(method, args);
                st.input_value(1,3);
                st();
            }else{
                StartCalculation<AnalisFactory,int,int> st(method, args);
                st.input_value(1,3);
                st();

            }
        }
    }
}

int main(int argc, char* argv[]) {
    
    ParserTerminal parser;
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
    exect(parser);
    return 0;
}
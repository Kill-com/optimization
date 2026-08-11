
#include "controller/start_controller/execution.hpp"
#include "controller/input_controller/parsers.hpp"
#include "controller/logger_controller/logger.hpp"
#include "analisis/executions.hpp"

void exect(ParserTerminal& parser){
    for (size_t i = 0; i < parser.getKeyCount(1); i++) {
        std::string method=parser.getKeyArg(i,1);
        for(size_t j=0; j<parser.getKeyCount(2);j++){
            std::string function=parser.getKeyArg(j,2);
            std::stringstream ss;
            ss<<"\n=================================\n"
            <<"Method name: "<<method<<". Function name: "<<function;
            logger->info(ss.str());
            std::initializer_list<std::string> args = {function};
            if(parser.get_for_analis()){
                StartCalculation<SimpleExect,float,float> st(method, args);
                st.input_value(1,3);
                st();
            }else{
                StartCalculation<AnalisFactory,float,float> st(method, args);
                st.input_value(1,3);
                st();

            }
        }
    }
}
int main(int argc, char* argv[]) {
    Logger* logger = &Logger::getInstance();
    auto consoleSub = std::make_shared<ConsoleLogCommand>();
    auto fileSub = std::make_shared<FileLogCommand>("app.log");
    logger->subscribe(LogLevel::INFO, consoleSub);
    logger->subscribe(LogLevel::INFO, fileSub);
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
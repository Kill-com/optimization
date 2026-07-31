#include "parsers.hpp"

void StorageParser::input_info(std::string i){
    info.push_back(i);
}
StorageParser& StorageParser::operator++(){
    info_size++;
    return *this;
}
std::vector<std::string>& StorageParser::output_info(){
    return info;
}
size_t StorageParser::output_size() const{
    return info_size;
}
void StorageParser::del_el(size_t index){
    info.erase(info.begin() + index); 
    info_size--;
}


IStorageParser::IStorageParser(const IStorageParser& parse){
    this->method=parse.method;
    this->target_function=parse.target_function;
}
const std::vector<std::string> IStorageParser::getKey(int i){
    switch (i){
    case 1: return method.output_info();
    case 2: return target_function.output_info();
    };
    return {};
}
size_t IStorageParser::getKeyCount(int i){
    switch (i){
    case 1: return method.output_size();
    case 2: return target_function.output_size();
    };
    return 0;
}
void IStorageParser::del_el(int i, size_t index){
    switch (i){
    case 1: method.del_el(index); break;
    case 2: target_function.del_el(index); break;
    }
}
std::string IStorageParser::getKeyArg(size_t index, int i){
    size_t size=getKeyCount(i);
    std::vector<std::string> args=getKey(i);
    return (index < size) ? args[index] : nullptr;
}

bool ParserTerminal::get_for_analis(){
    return for_analis;
}
bool ParserTerminal::parse(int argc, char**& argv){
    size_t estimated_args = (argc - 1) / 2;
    std::vector<std::string>& arg_method=method.output_info();
    std::vector<std::string>& arg_target_function=target_function.output_info();
    arg_method.reserve(estimated_args);
    arg_target_function.reserve(estimated_args);
    bool has_key2;
    if (argc == 1) return false;
    
    enum KeyType { NONE, KEY1, KEY2 };
    KeyType currentKey = NONE;
    
    for (int i = 1; i < argc; i++) {
        std::string arg= argv[i];
        
        if (arg == "-m" || arg == "--method") {
            currentKey = KEY1;
        } else if (arg == "-f" || arg == "--function") {
            currentKey = KEY2;
            has_key2 = true;
        } else if (arg == "-s") { 
            currentKey = NONE;  // Сбрасываем, так как у флага нет аргументов
            for_analis = true;
        } else {
            if (currentKey == KEY1) {
                arg_method.emplace_back(arg);
                ++method;
            } else if (currentKey == KEY2) {
                arg_target_function.emplace_back(arg);
                ++target_function;
            }
        }
    }
    
    return (has_key2 && !arg_target_function.empty());
}


namespace fs = std::filesystem;
bool Parser_file::parse(int m, std::vector<std::string>& arg){
    for (const std::string& fl : arg) {
        std::string filename=fl.data();
        if (!fs::exists(filename) && !fs::is_directory(filename)) {
            std::cerr << "Это папка не существует или не могу открыть!" << std::endl;
        }else{
            for (const auto& entry : fs::directory_iterator(filename)) {
                if(m==1){
                    method.input_info(filename+entry.path().filename().string());
                    ++method;
                }else if(m==2){
                    target_function.input_info(filename+entry.path().filename().string());
                    ++target_function;

                }
            }
        }
    }
    return false;
}



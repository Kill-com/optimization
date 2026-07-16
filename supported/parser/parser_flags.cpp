#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

#include "parser_flags.hpp"

bool Parser_terminal::parse(const std::tuple<int, char**>& args) {
    auto [argc, argv] =args;
    // Pre-allocate memory to avoid reallocations
    size_t estimated_args = (argc - 1) / 2;
    key1_args.reserve(estimated_args);
    key2_args.reserve(estimated_args);
    
    if (argc == 1) return false;
    
    enum KeyType { NONE, KEY1, KEY2 };
    KeyType currentKey = NONE;
    
    for (int i = 1; i < argc; i++) {
        std::string_view arg(argv[i]);
        
        if (arg == "-f" || arg == "--function") {
            currentKey = KEY1;
            has_key1 = true;
        } else if (arg == "-m" || arg == "--method") {
            currentKey = KEY2;
            has_key2 = true;
        } else if (arg == "-s") { 
            currentKey = NONE;  // Сбрасываем, так как у флага нет аргументов
            has_flag = true;
        } else {
            if (currentKey == KEY1) {
                key1_args.emplace_back(arg);
                key1_size++;
            } else if (currentKey == KEY2) {
                key2_args.emplace_back(arg);
                key2_size++;
            }
        }
    }
    
    return (has_key1 && !key1_args.empty());
}
void Parser_file::parse(const std::tuple<std::vector<std::string_view>>& args){
    auto [arg] =args;
    for (const std::string_view& fl : arg) {
        std::string filename=fl.data();
        if (!fs::exists(filename) && !fs::is_directory(filename)) {
            std::cout << "Это папка не существует или не могу открыть!" << std::endl;
        }else{
            for (const auto& entry : fs::directory_iterator(filename)) {
            count++;
            strings.push_back(entry.path().filename().string());
            }
        }
    }
}
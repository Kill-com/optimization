#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <filesystem>

namespace fs = std::filesystem;

//checking the file extension
bool hasExtension(const std::string_view& filename, const std::string& ext) {
    fs::path path(filename);
    return path.extension() == ext;
}

class OptimizedArgumentParser {
private:
    // Using string_view to avoid copying
    std::vector<std::string_view> key1_args;
    std::vector<std::string_view> key2_args;
    bool has_key1 = false;
    bool has_key2 = false;
    bool has_flag = false;
    
    // Caching sizes for fast access
    size_t key1_size = 0;
    size_t key2_size = 0;
    
public:
    bool parse(int argc, char* argv[]) {
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
            } else if (arg == "-s") {  // 👈 Новый ключ
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
    
    // Fast access with inlining
    inline const std::vector<std::string_view>& getKey1() const { 
        return key1_args; 
    }
    
    inline const std::vector<std::string_view>& getKey2() const { 
        return key2_args; 
    }
    
    inline size_t getKey1Count() const { return key1_size; }
    inline size_t getKey2Count() const { return key2_size; }
    
    // Index-based access (faster than vector iteration)
    inline std::string_view getKey1Arg(size_t index) const {
        return (index < key1_size) ? key1_args[index] : std::string_view();
    }
    
    inline std::string_view getKey2Arg(size_t index) const {
        return (index < key2_size) ? key2_args[index] : std::string_view();
    }
};

int main(int argc, char* argv[]) {
    OptimizedArgumentParser parser;
    
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
    
    return 0;
}
#pragma once
#include <vector>
#include <string>
#include <string_view>

class Parser {
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
    bool parse(int argc, char* argv[]);
    // Fast access with inlining
    inline const std::vector<std::string_view>& getKey1() const {return key1_args;};
    inline const std::vector<std::string_view>& getKey2() const {return key2_args;};
    inline size_t getKey1Count() const { return key1_size; }
    inline size_t getKey2Count() const { return key2_size; }
    inline const bool getflag() const {return has_flag; }
    // Index-based access (faster than vector iteration)
    inline std::string_view getKey1Arg(size_t index) const {
        return (index < key1_size) ? key1_args[index] : std::string_view();
    }
    
    inline std::string_view getKey2Arg(size_t index) const {
        return (index < key2_size) ? key2_args[index] : std::string_view();
    }
};
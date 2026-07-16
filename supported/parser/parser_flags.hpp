#pragma once
#include <iostream>

#include <vector>
#include <string>
#include <optional>
#include <string_view>



class Parser {
    public:
    virtual std::optional<bool> parse(const std::tuple<>& args){
        std::cerr << "Parser::parse (0 args)\n"<<std::endl;
        return false;
    };
    virtual std::string_view getKeyArg(size_t, int) const{
        std::cerr << "Parser::getKeyArg parrent none"<<std::endl;
        return "";
    };
    virtual size_t getKeyCount(int num=1) const{
    std::cerr << "Parser::getKeyCount parrent none"<<std::endl;
        return 0;
    };
    virtual ~Parser()=default;
};


class Parser_terminal:public Parser{
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
        bool parse(const std::tuple<int, char**>& args);
        inline const std::vector<std::string_view>& getKey(int num) const {
        if(num==1) return key1_args; else return key2_args;};
        size_t getKeyCount(int num=1) const { if (num==1) return key1_size; else return key2_size; }
        const bool getflag() const {return has_flag; }
        // Index-based access (faster than vector iteration)
        inline std::string_view getKeyArg(size_t index, int num) const {
            if(num==1)return (index < key1_size) ? key1_args[index] : std::string_view();
            else return (index < key2_size) ? key2_args[index] : std::string_view();
        }
        inline bool b_key(int num){
            if(num==1){
                return (has_key1 && !key1_args.empty());
            }else{
                return (has_key2 && !key2_args.empty());
            }
        }
};

class Parser_file:public Parser{
    private:
    std::vector<std::string_view> strings;
    size_t count=0;
    public:
    void parse(const std::tuple<std::vector<std::string_view>>&);
    inline const std::vector<std::string_view>& getstrings()const {return strings;};
    size_t getKeyCount(int k=1) const {return count;};
    std::string_view getKeyArg(size_t index, int num) const {
        return (index < count) ? strings[index] : std::string_view();
    }
};
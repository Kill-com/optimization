#pragma once
#include <iostream>

#include <vector>
#include <string>
#include <optional>

class Parser_terminal{
    private:
    bool has_key1 = false;
    bool has_key2 = false;
    bool has_flag = false;
    protected:
// Using string to avoid copying
    std::vector<std::string> key1_args;
    std::vector<std::string> key2_args;
    
    // Caching sizes for fast access
    size_t key1_size = 0;
    size_t key2_size = 0;
    public:
        Parser_terminal(){};
        Parser_terminal(const Parser_terminal& other){
            key1_args=other.key1_args;
            key2_args=other.key2_args;
            key1_size=other.key1_size;
            key2_size=other.key2_size;
        }

        virtual std::optional<bool> parse(const std::tuple<int, char**>& args);
        void del_el(int m,size_t index){
            if(m==1){
                key1_args.erase(key1_args.begin() + index); key1_size--;
            }else{
                key2_args.erase(key2_args.begin() + index); key2_size--;
            }
        }
        inline const std::vector<std::string>& getKey(int num) const {
        if(num==1) return key1_args; else return key2_args;};
        size_t getKeyCount(int num=1) const { if (num==1) return key1_size; else return key2_size; }
        const bool getflag() const {return has_flag; }
        // Index-based access (faster than vector iteration)
        inline std::string getKeyArg(size_t index, int num) const {
            if(num==1)return (index < key1_size) ? key1_args[index] : std::string();
            else return (index < key2_size) ? key2_args[index] : std::string();
        }
        inline bool b_key(int num){
            if(num==1){
                return (has_key1 && !key1_args.empty());
            }else{
                return (has_key2 && !key2_args.empty());
            }
        }
    virtual ~Parser_terminal()=default;
};

class Parser_file:public Parser_terminal{
    public:
    Parser_file(const Parser_terminal& parent): Parser_terminal(parent){}
    std::optional<bool> parse(const std::tuple<int, std::vector<std::string>>&);
};
#pragma once
#include <iostream>

#include <vector>
#include <string>

#include <filesystem>
#include <fstream>

class containerParser{
private:
    std::vector<std::string> info;
    size_t info_size = 0;
public:
    void input_info(std::string);
    containerParser& operator++();
    std::vector<std::string>& output_info();
    size_t output_size() const;
    void del_el(size_t);
};

class IcontainerParser{
protected:
    containerParser method;
    containerParser target_function;
public:
    IcontainerParser(): method(containerParser()),target_function(containerParser()){};
    IcontainerParser(const IcontainerParser&);
    const std::vector<std::string> getKey(int);
    void del_el(int, size_t);
    std::string getKeyArg(size_t, int);
    size_t getKeyCount(int);
};

template<typename T>
class IParser{
public:
    IParser(){};
    virtual bool parse(int,T&)=0;
    virtual ~IParser()=default;
};

class ParserTerminal:public IParser<char**>, public IcontainerParser{
private:
    bool for_analis = false;
    bool has_method = false;
    bool has_target_function = false;
public:
    ParserTerminal():IcontainerParser(){};
    bool get_for_analis();
    bool parse(int, char**&);
};

class Parser_file:public IParser<std::vector<std::string>>, public IcontainerParser{
    public:
    Parser_file(const IcontainerParser& parent): IcontainerParser(parent){};
    bool parse(int, std::vector<std::string>&);
};
#pragma once
#include <iostream>

#include <vector>
#include <string>

#include <filesystem>
#include <fstream>

class StorageParser{
private:
    std::vector<std::string> info;
    size_t info_size = 0;
public:
    void input_info(std::string);
    StorageParser& operator++();
    std::vector<std::string>& output_info();
    size_t output_size() const;
    void del_el(size_t);
};

class IStorageParser{
protected:
    StorageParser method;
    StorageParser target_function;
public:
    IStorageParser(): method(StorageParser()),target_function(StorageParser()){};
    IStorageParser(const IStorageParser&);
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

class ParserTerminal:public IParser<char**>, public IStorageParser{
private:
    bool for_analis = false;
    bool has_method = false;
    bool has_target_function = false;
public:
    ParserTerminal():IStorageParser(){};
    bool get_for_analis();
    bool parse(int, char**&);
};

class Parser_file:public IParser<std::vector<std::string>>, public IStorageParser{
    public:
    Parser_file(const IStorageParser& parent): IStorageParser(parent){};
    bool parse(int, std::vector<std::string>&);
};
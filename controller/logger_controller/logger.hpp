#pragma once

#include <fstream>


#include "interface.hpp"

// Команда реализации логирования в косоли
class ConsoleLogCommand : public LogSubscriber {
public:
     void onLogEvent(const LogEvent& event);
};

// Команда реализации логирования в файл
class FileLogCommand : public LogSubscriber{
public:
    FileLogCommand(const std::string& filename):file(filename, std::ios::app){};
    
     void onLogEvent(const LogEvent& event);
    
private:
    std::ofstream file;
};
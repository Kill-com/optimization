#pragma once

#include <fstream>
#include <iostream>

#include "interface.hpp"

// Команда реализации логирования в косоли
class ConsoleLogCommand : public LogSubscriber {
public:
    void onLogEvent(const LogEvent& event){
        std::cout << "[CONSOLE] " << getTimestamp() << " - " << event.toString() << std::endl;
    }
};

// Команда реализации логирования в файл
class FileLogCommand : public LogSubscriber{
public:
    FileLogCommand(const std::string& filename):file(filename, std::ios::app){};
    // Реализация файла
    void onLogEvent(const LogEvent& event){
        // Реализация записи в файл
        if (file.is_open()) {
            file << "[FILE] " << getTimestamp() << " - " << event.toString() << std::endl;
        }
    }
    
private:
    std::ofstream file;
};
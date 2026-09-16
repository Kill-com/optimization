#pragma once

#include <fstream>
#include <iostream>

#include "../controller_thread/GUI_controller.hpp"

#include "interface.hpp"

// Команда реализации логирования в консоли
class ConsoleLogCommand : public LogSubscriber {
public:
    void update(const LogEvent& event) override;
};

// Команда реализации логирования в файл
class FileLogCommand : public LogSubscriber {
public:
    explicit FileLogCommand(const std::string& filename)
        : file(filename, std::ios::app) {
        if (!file.is_open()) {
            throw std::runtime_error("FileLogCommand: cannot open " + filename);
        }
    }

    void update(const LogEvent& event) override;

private:
    std::ofstream file;
};

class GUILogCommand : public LogSubscriber {
public:
    void update(const LogEvent& event) override;
};
#include <chrono>
#include <iomanip>
#include <sstream>

#include "logger.hpp"

// Реализация интерфейса
std::string LogSubscriber::getTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void ConsoleLogCommand::update(const LogEvent& event) {
    try {
        std::cout << "[CONSOLE] " << getTimestamp()
                  << " - " << event.toString() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "ConsoleLogCommand error: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "ConsoleLogCommand unknown error" << std::endl;
    }
}

void FileLogCommand::update(const LogEvent& event) {
    try {
        if (file.is_open()) {
            file << "[FILE] " << getTimestamp()
                 << " - " << event.toString() << std::endl;
            file.flush();
        }
    } catch (const std::exception& e) {
        std::cerr << "FileLogCommand error: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "FileLogCommand unknown error" << std::endl;
    }
}

void GUILogCommand::update(const LogEvent& event) {
    try {
        GUI << event.getMessage();
    } catch (const std::exception& e) {
        std::cerr << "GUILogCommand error: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "GUILogCommand unknown error" << std::endl;
    }
}
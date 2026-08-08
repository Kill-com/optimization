

#include "interface.hpp"
#include <sstream>

//Реализация LogEvent
 std::string LogEvent::toString() const {
    std::stringstream ss;
    ss << "[" << levelToString(level_) << "] ";
    ss << message_;
    return ss.str();
}

std::string LogEvent::levelToString(LogLevel level) {
    switch(level) {
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::DEBUG: return "DEBUG";
        default: return "UNKNOWN";
    }
}
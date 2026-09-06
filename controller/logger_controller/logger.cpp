
#include <chrono>
#include <iomanip>

#include "logger.hpp"


//Реализация интерфейса
std::string LogSubscriber::getTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

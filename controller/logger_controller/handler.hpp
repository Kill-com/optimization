#pragma once

#include <string>
#include <memory>
#include <functional>
#include <map>
#include <mutex>

#include "logger.hpp"
// Издатель (логгер)
class Logger {
public:
    static Logger& getInstance();
    
    // Подписка на события
    void subscribe(LogLevel, std::shared_ptr<LogSubscriber>);
    void unsubscribe(LogLevel, std::shared_ptr<LogSubscriber>);
    
    // Публикация события
    void log(LogLevel, const std::string&);
    void info(const std::string& message) {
        log(LogLevel::INFO, message);
    }
private:
    Logger() = default;
    ~Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    std::map<LogLevel, std::vector<std::shared_ptr<LogSubscriber>>> subscribers_;
    std::mutex mutex_;
    
    void notify(LogLevel, const LogEvent&);
};
inline Logger* logger=nullptr;
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
    template<auto Name>
    void log(LogLevel level, const std::string& message){
        LogEvent event(level, message);
        event.updatemessage<Name>();
        notify(level, event);
    }
    void log(LogLevel, const std::string&);
    template<auto Name>
    void info(const std::string& message) {
        log<Name>(LogLevel::INFO, message);
    }
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
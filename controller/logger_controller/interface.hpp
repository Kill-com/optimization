#pragma once

#include <string>
#include <chrono>

// Типы событий для логирования
enum class LogLevel {
    INFO,
    WARNING,
    ERROR,
    DEBUG
};

class LogEvent {
public:
    LogEvent(LogLevel level, const std::string& message)
        : level_(level), message_(message) {
        timestamp_ = std::chrono::system_clock::now();
    }
    
    LogLevel getLevel() const { return level_; }
    std::string getMessage() const { return message_; }
    std::chrono::system_clock::time_point getTimestamp() const { return timestamp_; }
    
    std::string toString() const ;
private:
    LogLevel level_;
    std::string message_;
    std::chrono::system_clock::time_point timestamp_;
    
    static std::string levelToString(LogLevel);
};

// Интерфейс подписчика
class LogSubscriber {
protected:
    std::string getTimestamp() const;
public:
    virtual ~LogSubscriber() = default;
    virtual void onLogEvent(const LogEvent&) = 0;
};
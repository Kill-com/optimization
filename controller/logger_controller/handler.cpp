
#include <sstream>
#include <algorithm>

#include "handler.hpp"

//Реализация Издатель (логгер)
Logger& Logger::getInstance() {
    if(logger==nullptr){
        logger = new Logger;
    }
    return *logger;
}

void Logger::subscribe(LogLevel level, std::shared_ptr<LogSubscriber> subscriber) {
    std::lock_guard<std::mutex> lock(mutex_);
    subscribers_[level].push_back(subscriber);
}

void Logger::unsubscribe(LogLevel level, std::shared_ptr<LogSubscriber> subscriber) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto& list = subscribers_[level];
    list.erase(std::remove(list.begin(), list.end(), subscriber), list.end());
}

// Публикация события
void Logger::log(LogLevel level, const std::string& message) {
    LogEvent event(level, message);
    notify(level, event);
}
void Logger::notify(LogLevel level, const LogEvent& event) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // Уведомляем подписчиков на этом уровне
    auto it = subscribers_.find(level);
    if (it != subscribers_.end()) {
        for (auto& subscriber : it->second) {
            if (subscriber) {
                subscriber->onLogEvent(event);
            }
        }
    }
    
    // Уведомляем подписчиков на всех уровнях (если нужны)
    it = subscribers_.find(LogLevel::DEBUG); // DEBUG как уровень для всех
    if (it != subscribers_.end()) {
        for (auto& subscriber : it->second) {
            if (subscriber) {
                subscriber->onLogEvent(event);
            }
        }
    }
}
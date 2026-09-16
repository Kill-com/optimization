#pragma once

#include <string>
#include <memory>
#include <functional>
#include <map>
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <sstream>
#include <iostream>

#include "logger.hpp"

class Logger {
public:
    static Logger& getInstance();

    void subscribe(LogLevel, std::shared_ptr<LogSubscriber>);
    void unsubscribe(LogLevel, std::shared_ptr<LogSubscriber>);

    // ---- API, как было ----
    template<auto Name>
    void log(LogLevel level, const std::string& message) {
        // Копируем всё в задачу, чтобы вызывающий поток не ждал
        enqueue([this, level, message]() {
            LogEvent event(level, message);
            event.template updatemessage<Name>();
            notify(level, event);
        });
    }

    void log(LogLevel level, const std::string& message) {
        enqueue([this, level, message]() {
            LogEvent event(level, message);
            notify(level, event);
        });
    }

    template<auto Name>
    void info(const std::string& message) {
        log<Name>(LogLevel::INFO, message);
    }

    void info(const std::string& message) {
        log(LogLevel::INFO, message);
    }

    template<typename T>
    void method_log(T message) {
        enqueue([this, message]() {
            std::ostringstream os;
            os << message;
            // Копируем строку — она переживёт выход из метода
            notify(LogLevel::INFO, LogEvent(LogLevel::INFO, os.str()));
        });
    }

    // Явно дождаться обработки всей очереди (например, при выходе)
    void flush();

    ~Logger();

private:
    Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    using Task = std::function<void()>;
    void enqueue(Task t);
    void worker_loop();
    void notify(LogLevel, const LogEvent&);

    std::map<LogLevel, std::vector<std::shared_ptr<LogSubscriber>>> subscribers_;
    std::mutex mutex_;                 // для subscribers_

    std::queue<Task> queue_;
    std::mutex queue_mutex_;
    std::condition_variable queue_cv_;
    std::thread worker_;
    std::atomic<bool> stopping_{false};
    std::atomic<int> in_flight_{0};
};

inline Logger* logger=nullptr;
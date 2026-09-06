#pragma once
#include <memory>
#include <vector>
#include <string>
#include <chrono>

#include "../../analisis/name_iter.hpp"

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
    template<auto Name>
    void updatemessage(){
        message_=how_iter<Name>()+message_;
    }
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

//базовый класс логируемых объектов
class ToLog{
public:
    virtual ~ToLog()=default;
    virtual void tolog()=0;
    virtual void reset()=0;
};

//Класс для логируемых обьектов
class ConteinerLog{
private:
    static std::vector<std::shared_ptr<ToLog>> commands;

public:
    // Для передачи существующих объектов в список
    template<class T>
    static void input_command(std::shared_ptr<T> cl) {
        if (cl) {
            commands.push_back(cl);
        }
    }
    
    // Для создания новых объектов (НЕ ИСПОЛЬЗУЙТЕ С UNIQUE_PTR!)
    template<class T, typename... Args>
    static void add_command(Args&&... args) {
        commands.push_back(std::make_shared<T>(std::forward<Args>(args)...));
    }
        
    static void reset(){
        // ВНИМАНИЕ: Удаляем объекты, если они были созданы через add_command
        for (auto& cmd : commands) {
            cmd->reset();
        }
        commands.clear();
    }
    
    //старт логирования
    static void startlog(){
        for (auto& cmd : commands) {
            if (cmd) {
                cmd->tolog();
            }
        }
    }
};

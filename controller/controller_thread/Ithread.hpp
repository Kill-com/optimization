#pragma once

#include <mutex>
#include <thread>

#include "../controller_thread/GUI_controller.hpp"
#include "../logger_controller/handler.hpp"

class ThreadLOG{
protected:
    inline static Logger* logger = &Logger::getInstance();
};
#define LOCK std::lock_guard<std::mutex> lock(mx);
class ThreadTask:public ThreadLOG{
protected:
    std::mutex mx;
    std::thread ex;
public:
    ~ThreadTask(){
        logger->flush();
        GUI.close();
    }
};
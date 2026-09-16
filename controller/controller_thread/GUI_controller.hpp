

/**
 * Тут должен быть класс для управление потоками с GUI и передача в exect и получение с logger
 * 
 */

 // message_stream.hpp
#pragma once

#include <mutex>
#include <queue>
#include <condition_variable>
#include <string>
#include <sstream>
#include <thread>
#include <iostream>

class MessageStream {
    std::mutex mtx;
    std::condition_variable cv;
    std::queue<std::string> q;
    bool closed = false;

public:
    // каждый << -> отдельное сообщение
    template<typename T>
    MessageStream& operator<<(const T& v) {
        std::ostringstream os;
        os << v;                       // формируем кусок
        {
            std::lock_guard<std::mutex> lk(mtx);
            q.push(os.str());          // копия в очередь
        }
        cv.notify_one();
        return *this;
    }

    bool recv(std::string& out) {
        std::unique_lock<std::mutex> lk(mtx);
        cv.wait(lk, [&]{ return !q.empty() || closed; });
        if (q.empty()) return false;
        out = q.front();
        q.pop();
        return true;
    }

    void close() {
        { std::lock_guard<std::mutex> lk(mtx); closed = true; }
        cv.notify_all();
    }
};

inline MessageStream GUI;

#include <sstream>
#include <algorithm>

#include "handler.hpp"

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

Logger::Logger() {
    worker_ = std::thread([this] { worker_loop(); });
}

Logger::~Logger() {
    {
        std::lock_guard<std::mutex> lk(queue_mutex_);
        stopping_ = true;
    }
    queue_cv_.notify_all();
    if (worker_.joinable()) worker_.join();
}

void Logger::enqueue(Task t) {
    {
        std::lock_guard<std::mutex> lk(queue_mutex_);
        if (stopping_) return;
        queue_.push(std::move(t));
        in_flight_.fetch_add(1, std::memory_order_relaxed);
    }
    queue_cv_.notify_one();
}

void Logger::worker_loop() {
    while (true) {
        Task task;
        {
            std::unique_lock<std::mutex> lk(queue_mutex_);
            queue_cv_.wait(lk, [this] { return stopping_ || !queue_.empty(); });
            if (stopping_ && queue_.empty()) return;
            task = std::move(queue_.front());
            queue_.pop();
        }
        try { task(); } catch (...) { /* ... */ }
        in_flight_.fetch_sub(1, std::memory_order_relaxed);
        queue_cv_.notify_all();          // разбудить flush
    }
}

void Logger::flush() {
    std::unique_lock<std::mutex> lk(queue_mutex_);
    queue_cv_.wait(lk, [this] {
        return queue_.empty()
            && in_flight_.load(std::memory_order_relaxed) == 0;
    });
}

void Logger::notify(LogLevel level, const LogEvent& event) {
    std::vector<std::shared_ptr<LogSubscriber>> snapshot;
    {
        std::lock_guard<std::mutex> lk(mutex_);
        auto it = subscribers_.find(level);
        if (it == subscribers_.end()) return;
        snapshot = it->second;   // копия shared_ptr — дёшево
    }
    for (auto& sub : snapshot) {
        if (sub) {
            try { sub->update(event); }
            catch (const std::exception& e) {
                std::cerr << "Subscriber error: " << e.what() << std::endl;
            }
            catch (...) {
                std::cerr << "Subscriber unknown error" << std::endl;
            }
        }
    }
}
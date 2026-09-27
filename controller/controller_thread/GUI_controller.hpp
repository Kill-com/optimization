#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <utility>


// канал между вычислительной частью и GUI
class MessageStream {
private:
    mutable std::mutex mtx;
    std::condition_variable cv;
    std::queue<GUIData> q;
    bool closed = false;

public:
    MessageStream() = default;

    MessageStream(const MessageStream&) = delete;
    MessageStream& operator=(const MessageStream&) = delete;

    template<typename T>
    MessageStream& operator<<(const T& value) {
        std::ostringstream stream;
        stream << value;
        send(stream.str());
        return *this;
    }

    void send(std::string value) {
        {
            std::lock_guard<std::mutex> lock(mtx);

            if (closed) {
                return;
            }

            q.emplace(std::move(value));
        }

        cv.notify_one();
    }

    bool recv(GUIData& out) {
        std::unique_lock<std::mutex> lock(mtx);

        cv.wait(lock, [this] {
            return !q.empty() || closed;
        });

        if (q.empty()) {
            return false;
        }

        out = std::move(q.front());
        q.pop();
        return true;
    }

    void close() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            closed = true;
        }

        cv.notify_all();
    }
};

// общий канал gui
inline MessageStream GUI;

// Контроллер GUI-потока
class GUIController {
public:
    using Data = GUIData;
    using MessageHandler = void (*)(const Data&);

private:
    std::thread guiThread;
    MessageHandler handler = nullptr;

    // Текущий выбранный график
    std::string currentPlot;

public:
    explicit GUIController(MessageHandler messageHandler = nullptr)
        : handler(messageHandler) {
        start();
    }

    GUIController(const GUIController&) = delete;
    GUIController& operator=(const GUIController&) = delete;

    ~GUIController() {
        stop();
    }

    // выбор графика
    void setPlot(std::string name) {
        currentPlot = std::move(name);
    }

    const std::string& getPlot() const {
        return currentPlot;
    }

    //передача координатт точки
    void plot(double x, double y) {
        std::ostringstream stream;
        stream << '(' << x << ':' << y << ')';
        GUI.send(stream.str());
    }

    void start() {
        if (guiThread.joinable()) {
            return;
        }
        guiThread = std::thread([this] {
            run();
        });
    }

    void stop() {
        if (!guiThread.joinable()) {
            return;
        }

        GUI.close();

        if (guiThread.joinable()) {
            guiThread.join();
        }
    }

private:
    void run() {
        Data data;

        while (GUI.recv(data)) {
            if (handler != nullptr) {
                handler(data);
            }
        }
    }
};

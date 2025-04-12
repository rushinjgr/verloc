#pragma once
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <chrono>
#include <condition_variable>

class BaseLooper {
protected:
    std::string name;
    std::chrono::milliseconds interval;
    std::map<std::string, std::shared_ptr<BaseLooper>> subprocesses;
    std::mutex mtx;
    bool exitFlag = false;
    std::condition_variable exitCond;

    // Protected helper methods.
    void addSubprocess(std::string name, std::shared_ptr<BaseLooper> subprocess);
    void removeSubprocess(std::string name);

public:
    BaseLooper(std::string name, std::chrono::milliseconds interval);
    virtual ~BaseLooper();

    virtual void setup();
    virtual void teardown();
    virtual void run();

    // Virtual initialization method that derived classes can override
    // to perform additional initialization (like adding subprocesses).
    virtual void initialize() { }

    // Factory method: create an instance of any derived type.
    template <typename T, typename... Args>
    static std::shared_ptr<T> create(Args&&... args) {
        auto instance = std::make_shared<T>(std::forward<Args>(args)...);
        instance->initialize();
        // Start setup() in a detached thread so that creation returns immediately.
        std::thread([instance]() { instance->setup(); }).detach();
        return instance;
    }
};

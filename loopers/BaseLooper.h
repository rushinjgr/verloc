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

    // These methods are now protected
    void addSubprocess(std::string name, std::shared_ptr<BaseLooper> subprocess);
    void removeSubprocess(std::string name);

public:
    BaseLooper(std::string name, std::chrono::milliseconds interval);
    virtual ~BaseLooper();

    virtual void setup();
    virtual void teardown();
    virtual void run();
};

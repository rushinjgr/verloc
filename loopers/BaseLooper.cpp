// BaseLooper.cpp
#include "BaseLooper.h"

BaseLooper::BaseLooper(std::string name, std::chrono::milliseconds interval)
    : name(name), interval(interval) {}

BaseLooper::~BaseLooper() {}

void BaseLooper::setup() {
    std::cout << "[" << name << "] Performing setup..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "[" << name << "] Setup complete!" << std::endl;
}

void BaseLooper::teardown() {
    std::unique_lock<std::mutex> lock(mtx);
    exitFlag = true;
    exitCond.notify_all();
    std::cout << "[" << name << "] Performing teardown..." << std::endl;

    for (auto& [name, subprocess] : subprocesses) {
        std::cout << "[" << name << "] Stopping subprocess: " << name << std::endl;
        subprocess->teardown();
    }

    std::cout << "[" << name << "] Teardown complete!" << std::endl;
}

void BaseLooper::run() {
    std::cout << "[" << name << "] Loop running every " << interval.count() << "ms... Press Ctrl+C to exit." << std::endl;

    while (!exitFlag) {
        std::this_thread::sleep_for(interval);
        std::cout << "[" << name << "] Instance doing work..." << std::endl;
    }
}

void BaseLooper::addSubprocess(std::string name, std::shared_ptr<BaseLooper> subprocess) {
    std::unique_lock<std::mutex> lock(mtx);
    subprocesses[name] = subprocess;
    std::thread([subprocess]() { subprocess->run(); }).detach();
}

void BaseLooper::removeSubprocess(std::string name) {
    std::unique_lock<std::mutex> lock(mtx);
    subprocesses.erase(name);
}
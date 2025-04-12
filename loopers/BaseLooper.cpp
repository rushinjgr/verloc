#include "BaseLooper.h"
#include <thread>

BaseLooper::BaseLooper(std::string name, std::chrono::milliseconds interval)
    : name(name), interval(interval) {
    // Note: we do not call setup() automatically here.
}

BaseLooper::~BaseLooper() {}

void BaseLooper::setup() {
    std::cout << "[" << name << "] Performing setup..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "[" << name << "] Setup complete!" << std::endl;

    {
        std::unique_lock<std::mutex> lock(mtx);
        // Start each subprocess (if any) in its own thread.
        for (auto& pair : subprocesses) {
            auto subName = pair.first;
            auto subprocess = pair.second;
            std::cout << "[" << name << "] Starting subprocess: " << subName << std::endl;
            std::thread([subprocess]() { subprocess->run(); }).detach();
        }
    }

    // Now enter the main loop.
    this->run();
}

void BaseLooper::teardown() {
    std::unique_lock<std::mutex> lock(mtx);
    exitFlag = true;
    exitCond.notify_all();
    std::cout << "[" << name << "] Performing teardown..." << std::endl;

    for (auto& [subName, subprocess] : subprocesses) {
        std::cout << "[" << name << "] Stopping subprocess: " << subName << std::endl;
        subprocess->teardown();
    }
    std::cout << "[" << name << "] Teardown complete!" << std::endl;
}

void BaseLooper::run() {
    std::cout << "[" << name << "] Loop running every " << interval.count()
              << "ms... Press Ctrl+C to exit." << std::endl;

    while (!exitFlag) {
        std::this_thread::sleep_for(interval);
        std::cout << "[" << name << "] Instance doing work..." << std::endl;
    }

    std::cout << "[" << name << "] Exiting loop..." << std::endl;
    // Optionally, you might not want to call teardown() here if shutdown is managed externally.
    // teardown();
}

void BaseLooper::addSubprocess(std::string subName, std::shared_ptr<BaseLooper> subprocess) {
    std::unique_lock<std::mutex> lock(mtx);
    subprocesses[subName] = subprocess;
}

void BaseLooper::removeSubprocess(std::string subName) {
    std::unique_lock<std::mutex> lock(mtx);
    subprocesses.erase(subName);
}

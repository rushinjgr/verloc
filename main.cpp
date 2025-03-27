#include "loopers/MasterLooper.h"
#include <memory>
#include <csignal>
#include <atomic>

// Global flag to handle exit
std::atomic<bool> exitSignalReceived(false);

// Signal handler for multiple signals
void handleSignal(int signal) {
    switch (signal) {
        case SIGINT:
            exitSignalReceived.store(true);
            std::cout << "\n[global] SIGINT received. Terminating loopers...\n";
            break;
        case SIGTERM:
            exitSignalReceived.store(true);
            std::cout << "\n[global] SIGTERM received. Gracefully shutting down...\n";
            break;
        case SIGHUP:
            std::cout << "\n[global] SIGHUP received. Refreshing configuration...\n";
            // Add logic for refreshing or reinitializing resources here
            break;
        case SIGUSR1:
            std::cout << "\n[global] SIGUSR1 received. Custom action triggered...\n";
            // Add custom signal handling logic here
            break;
        case SIGUSR2:
            std::cout << "\n[global] SIGUSR2 received. Another custom action triggered...\n";
            // Add custom signal handling logic here
            break;
        default:
            std::cout << "\n[global] Unknown signal (" << signal << ") received.\n";
    }
}

int main() {
    // Attach signal handlers for multiple signals
    std::signal(SIGINT, handleSignal);
    std::signal(SIGTERM, handleSignal);
    std::signal(SIGHUP, handleSignal);
    std::signal(SIGUSR1, handleSignal);
    std::signal(SIGUSR2, handleSignal);

    auto masterLooper = std::make_shared<MasterLooper>();
    masterLooper->startNetworkLooper();

    // Run MasterLooper process
    std::thread masterThread([&]() {
        masterLooper->runMasterProcess();
    });

    // Monitor for exit signal
    while (!exitSignalReceived.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Trigger graceful shutdown
    masterLooper->teardown();

    // Wait for master thread to finish
    if (masterThread.joinable()) {
        masterThread.join();
    }

    std::cout << "[global] All loopers exited gracefully. Goodbye!" << std::endl;
    return 0;
}

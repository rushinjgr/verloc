#include "loopers/MasterLooper.h"
#include "util/sig_util.h"
#include <memory>
#include <csignal>
#include <thread>

// Signal handlers for multiple signals.
int main() {
    // Attach signal handlers.
    std::signal(SIGINT, handleSignal);
    std::signal(SIGTERM, handleSignal);
    std::signal(SIGHUP, handleSignal);
    std::signal(SIGUSR1, handleSignal);
    std::signal(SIGUSR2, handleSignal);

    // Use the factory method to create and initialize MasterLooper.
    auto masterLooper = BaseLooper::create<MasterLooper>();
    
    // Now that the looper is running in background threads, the main thread
    // can monitor for the exit signal.
    while (!exitSignalReceived.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    // Trigger graceful shutdown.
    masterLooper->teardown();
    
    std::cout << "[global] All loopers exited gracefully. Goodbye!" << std::endl;
    return 0;
}

#include "loopers/MasterLooper.h"
#include "util/sig_util.h"
#include <memory>
#include <csignal>
#include <thread>
#include <iostream>

int main() {
    // Attach all signal handlers using the utility function.
    setupSignalHandlers();

    // Use the factory method to create and initialize MasterLooper.
    auto masterLooper = BaseLooper::create<MasterLooper>();
    
    // Monitor the exit flag from the global utility.
    while (!exitSignalReceived.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    // Trigger graceful shutdown.
    masterLooper->teardown();
    
    std::cout << "[global] All loopers exited gracefully. Goodbye!" << std::endl;
    return 0;
}

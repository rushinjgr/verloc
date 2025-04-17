#include "sig_util.h"
#include <iostream>
#include <csignal>

// Define the global exit flag.
std::atomic<bool> exitSignalReceived(false);

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

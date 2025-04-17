#pragma once
#include <atomic>

// Global exit flag that will be set when a termination signal is received.
extern std::atomic<bool> exitSignalReceived;

// The signal handler function declaration.
void handleSignal(int signal);

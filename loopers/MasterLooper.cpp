#include "MasterLooper.h"

MasterLooper::MasterLooper()
    : BaseLooper("master", std::chrono::seconds(5)) {
    // Do not call startNetworkLooper or setup() here.
}

void MasterLooper::startNetworkLooper() {
    // Create a new NetIntLooper instance.
    auto netIntLooper = std::make_shared<NetIntLooper>();
    // Add it as a subprocess.
    addSubprocess("NetInt", netIntLooper);
    std::cout << "[master] Added NetIntLooper as a subprocess." << std::endl;
}

void MasterLooper::initialize() {
    // MasterLooper-specific initialization: add the network looper.
    startNetworkLooper();
}

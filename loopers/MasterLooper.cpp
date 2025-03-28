#include "MasterLooper.h"

MasterLooper::MasterLooper()
    : BaseLooper("master", std::chrono::seconds(5)) {}

void MasterLooper::runMasterProcess() {
    BaseLooper::run(); // Reuse the BaseLooper's run method
}

void MasterLooper::startNetworkLooper() {
    // Create a new NetIntLooper with a 2-second interval
    auto netIntLooper = std::make_shared<NetIntLooper>();

    // Call the inherited addSubprocess method
    addSubprocess("NetInt", netIntLooper);

    std::cout << "[master] Added NetIntLooper as a subprocess." << std::endl;
}

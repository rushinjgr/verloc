#pragma once
#include "BaseLooper.h"
#include "NetIntLooper.h"

class MasterLooper : public BaseLooper {
public:
    MasterLooper(); // No need for an interval argument
    void runMasterProcess();
    void startNetworkLooper(); // New method to add a NetIntLooper
};

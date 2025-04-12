#pragma once
#include "BaseLooper.h"
#include "NetIntLooper.h"

class MasterLooper : public BaseLooper {
public:
    MasterLooper(); // Minimal constructor.
    void startNetworkLooper(); // Adds a NetIntLooper as a subprocess.

    // Override initialize() to perform derived-class initialization.
    virtual void initialize() override;
};

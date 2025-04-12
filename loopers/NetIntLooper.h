#pragma once
#include "BaseLooper.h"

class NetIntLooper : public BaseLooper {
public:
    NetIntLooper();
    // Override run() to implement network-specific logic.
    void run() override;
};

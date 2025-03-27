#pragma once
#include "BaseLooper.h"

class NetIntLooper : public BaseLooper {
public:
    NetIntLooper();
    // Declare the overridden run method
    void run() override;
};

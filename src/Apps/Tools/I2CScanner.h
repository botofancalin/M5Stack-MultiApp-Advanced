#pragma once
#include "M5Stack.h"

class I2CScannerClass
{
public:
    I2CScannerClass();
    ~I2CScannerClass();

    void Run();

private:
    uint8_t error;
    uint8_t address;
    uint8_t ridx = 0;
    uint8_t lidx = 0;
    unsigned int nDevices;
    bool scanrun = HIGH;
};


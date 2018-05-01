#pragma once
#include "M5Stack.h"

class CfgBrightnessClass
{
public:
    CfgBrightnessClass();
    ~CfgBrightnessClass();

    void Run();

private:
    unsigned short tmp_brigth;
    unsigned short tmp_lbrigth;
    int val;
};
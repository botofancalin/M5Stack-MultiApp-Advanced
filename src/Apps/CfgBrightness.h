#ifndef CfgBrightness_h
#define CfgBrightness_h

#include "M5StackSAM.h"

class CfgBrightnessClass
{
public:
    CfgBrightnessClass();
    ~CfgBrightnessClass();

    void Run();

private:
    unsigned short tmp_brigth;
    unsigned short tmp_lbrigth;
};
#endif
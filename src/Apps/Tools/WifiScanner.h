#pragma once
#include "M5StackMod.h"

class WifiScannerClass
{
public:
    WifiScannerClass();
    ~WifiScannerClass();

    void Run();

private:
    int wifi_count = 0;
    bool wifi_showlock = LOW;
    short list_lines = 5;
    int list_page = 0;
    int list_pages = 0;
    int list_lastpagelines = 0;
    String Encryption = "Unknown";
};


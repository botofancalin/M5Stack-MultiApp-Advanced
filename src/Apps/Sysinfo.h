#ifndef Sysinfo_h
#define Sysinfo_h
#include "M5StackSAM.h"

#define PAGEMAX 1

class SysinfoClass
{
  public:
    SysinfoClass();
    ~SysinfoClass();

    void Run();

  private:
    uint8_t page = 0;
    bool done = false;
    uint8_t baseMac[6];
    char baseMacChr[18] = {0};
    int chipRev;
    int cpuSpeed;
    int flashSpeed;
    int ramFree;
    String WiFiMAC;
    int flashSize;
    int type;

    String getWiFiMac();
    void page_0();
    void page_1();
    void drawpage(int page);
};

#endif
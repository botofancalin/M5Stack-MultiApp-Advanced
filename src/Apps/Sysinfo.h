#pragma once
#include "M5StackMod.h"

#define PAGEMAX 1

class SysinfoClass
{
public:
  SysinfoClass();
  ~SysinfoClass();

  void Run();

private:
  bool done = false;
  uint8_t page = 0;
  uint8_t baseMac[6];
  char baseMacChr[18] = {0};
  int chipRev;
  int cpuSpeed;
  int flashSpeed;
  int ramFree;
  int flashSize;
  int type;
  String WiFiMAC;
  unsigned long sdSize;
  unsigned long sdUsed;
  String SD_Type[5] = {"NONE", "MMC", "SD", "SDHC", "UNKNOWN"};
  String wifi_m_mode[5] = {"OFF", "STA", "AP", "AP STA", "MAX"};

  String getWiFiMac();
  void page_0();
  void page_1();
  void drawpage(int page);
};
#ifndef wrappers_h
#define wrappers_h

#include "Apps/About.h"
#include "Apps/CfgBrightness.h"
#include "Apps/Oscilloscope.h"
#include "Apps/SdBrowser.h"
#include "Apps/Sysinfo.h"
#include "Apps/WiFiSettings.h"
#include "Apps/MyWebServer.h"
#include "Apps/GamesList.h"

void appReturn();
void appSleep();
void appAbout();
void appCfgBrigthness();
void appOscilloscope();
void appSdBrowser();
void appSysInfo();
void appWiFiSetup();
void appWebServer();
void appGamesList();

#endif
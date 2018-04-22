#include "Wrappers.h"

void appReturn()
{
}

void appSleep()
{
    M5.setWakeupButton(BUTTON_B_PIN);
    M5.powerOFF();
}

void appAbout()
{
    AboutClass AboutObj ;
    AboutObj.Run();
}

void appCfgBrigthness()
{
    CfgBrightnessClass BrightnessObj;
    BrightnessObj.Run();
}

void appOscilloscope()
{
    OscilloscopeClass OscilloscopeObj;
    OscilloscopeObj.Run();
}

void appSdBrowser()
{
    SdBrowserClass SdBrowserObj;
    SdBrowserObj.Run();
}

void appSysInfo()
{
    SysinfoClass SysinfoObj;
    SysinfoObj.Run();
}

void appWiFiSetup()
{
    WifiSettingsClass WifiSettingsObj;
    WifiSettingsObj.Run();
}
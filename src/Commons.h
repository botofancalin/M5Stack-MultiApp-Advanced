#ifndef commons_h
#define commons_h

#include "WiFiUdp.h"
#include "M5Stack.h"
#include "EEPROM.h"
#include "M5StackSAM.h"
#include "esp_wifi.h"
#include "esp_wps.h"
#include "DHTesp.h"
#include "ArduinoOTA.h"
#include "WebServer.h"
#include "Preferences.h"
#include "AudioFileSourceSD.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include "AudioFileSourceICYStream.h"
#include "AudioFileSourceBuffer.h"
#include "Wrappers.h"

// add app external call function here to be able to call it from menu
void appOta();
void appWebServer();
void appStopWatch();
void appListTools();
void appListGames();
void WebRadio();

void appI2CScanner();
void WiFiScanner_run();
void Monitor_run();
void Dht_Run();

void spaceShootest_run();
void flappypird_run();

void meters_setup();
void meters_run();

#endif // !commons_h

#pragma once
#include "M5StackSam.h"

class M5StackServerOta
{
public:
void checkForNewVersion();
    M5StackServerOta(const int *ota_version);
    ~M5StackServerOta();

private:
String host = "nsteam.org";
String bin = "/M5Stack/SuperApp.bin"; // path to firmware binary.
String fwUrlBase = "http://www.nsteam.org/M5Stack/SuperApp.version";
int contentLength = 0, port = 80, version;
bool isValidContentType = false;
String getHeaderValue(String header, String headerName);
void execOTA();
};


#ifndef wpsconnect_h
#define wpsconnect_h

#include "M5Stack.h"


void Wps_run(bool mode);

String wpspin2string(uint8_t a[]);
static void WiFiEvent(WiFiEvent_t event, system_event_info_t info);

#endif // !wpsconnect_h
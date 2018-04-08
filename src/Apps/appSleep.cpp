#include "../apps.h"

void appSleep()
{
    M5.setWakeupButton(BUTTON_B_PIN);
    M5.powerOFF();
}
#include "CfgBrightness.h"

void CfgBrightnessClass::Run()
{
    preferences.begin("Brightness", false);
    tmp_bright = preferences.getUShort("light", 95);
    tmp_lbright = 0;

    while (!M5m.BtnB.wasPressed())
    {
        if (M5m.BtnA.wasPressed() && tmp_bright >= 16)
        {
            tmp_bright = tmp_bright - 16;
        }
        if (M5m.BtnC.wasPressed() && tmp_bright <= 239)
        {
            tmp_bright = tmp_bright + 16;
        }
        if (tmp_lbright != tmp_bright)
        {
            tmp_lbright = tmp_bright;
            preferences.putUShort("light", tmp_lbright);
            M5m.Lcd.setBrightness(tmp_lbright);
            M5m.windowClr();
            val = map(tmp_lbright, 16, 255, 0, 100);
            M5m.Lcd.HprogressBar(40, 110, 240, 30, YELLOW, val, true);
        }
        M5m.update();
    }
    preferences.end();
}

CfgBrightnessClass::CfgBrightnessClass()
{
    M5m.drawAppMenu(F("DISPLAY BRIGHTNESS"), F("-"), F("OK"), F("+"));
    M5m.update();
}

CfgBrightnessClass::~CfgBrightnessClass()
{
    M5m.show();
}

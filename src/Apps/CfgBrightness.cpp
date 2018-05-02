#include "CfgBrightness.h"

void CfgBrightnessClass::Run()
{
    preferences.begin("Brightnes", false);
    tmp_brigth = preferences.getUShort("light", 95);
    tmp_lbrigth = 0;

    while (!M5m.BtnB.wasPressed())
    {
        if (M5m.BtnA.wasPressed() && tmp_brigth >= 16)
        {
            tmp_brigth = tmp_brigth - 16;
        }
        if (M5m.BtnC.wasPressed() && tmp_brigth <= 239)
        {
            tmp_brigth = tmp_brigth + 16;
        }
        if (tmp_lbrigth != tmp_brigth)
        {
            tmp_lbrigth = tmp_brigth;
            preferences.putUShort("light", tmp_lbrigth);
            M5m.Lcd.setBrightness(tmp_lbrigth);
            M5m.windowClr();
            val = map(tmp_lbrigth, 16, 255, 0, 100);
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
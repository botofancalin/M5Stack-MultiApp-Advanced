#include "CfgBrightness.h"

void CfgBrightnessClass::Run()
{
    preferences.begin("Brightnes", false);
    tmp_brigth = preferences.getUShort("light", 95);
    tmp_lbrigth = 0;

    while (!M5.BtnB.wasPressed())
    {
        if (M5.BtnA.wasPressed() && tmp_brigth >= 16)
        {
            tmp_brigth = tmp_brigth - 16;
        }
        if (M5.BtnC.wasPressed() && tmp_brigth <= 239)
        {
            tmp_brigth = tmp_brigth + 16;
        }
        if (tmp_lbrigth != tmp_brigth)
        {
            tmp_lbrigth = tmp_brigth;
            preferences.putUShort("light", tmp_lbrigth);
            M5.Lcd.setBrightness(tmp_lbrigth);
            MyMenu.windowClr();
            val = map(tmp_lbrigth, 16, 255, 0, 100);
            M5.Lcd.HprogressBar(40, 110, 240, 30, YELLOW, val, true);
        }
        M5.update();
    }
    preferences.end();
}

CfgBrightnessClass::CfgBrightnessClass()
{
    MyMenu.drawAppMenu(F("DISPLAY BRIGHTNESS"), F("-"), F("OK"), F("+"));
    M5.update();
}

CfgBrightnessClass::~CfgBrightnessClass()
{
    MyMenu.show();
}
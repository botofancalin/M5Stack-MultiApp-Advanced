#include "CfgBrightness.h"

void CfgBrightnessClass::Run()
{
    preferences.begin("Brightness", false);
    tmp_brigth = preferences.getUShort("Brightness", 95);
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
            preferences.putUShort("Brightness", tmp_lbrigth);
            M5.Lcd.setBrightness(tmp_lbrigth);
            MyMenu.windowClr();
            M5.Lcd.drawNumber(tmp_lbrigth, 120, 90, 6);
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
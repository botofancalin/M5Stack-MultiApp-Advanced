#include "../apps.h"

void appCfgBrigthness()
{
    byte tmp_brigth = byte(EEPROM.read(0));
    byte tmp_lbrigth = 0;

    MyMenu.drawAppMenu(F("DISPLAY BRIGHTNESS"), F("-"), F("OK"), F("+"));

    while (M5.BtnB.wasPressed())
    {
        M5.update();
    }
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
            EEPROM.write(0, tmp_lbrigth);
            EEPROM.commit();
            M5.lcd.setBrightness(byte(EEPROM.read(0)));
            MyMenu.windowClr();
            M5.Lcd.drawNumber(byte(EEPROM.read(0)), 120, 90, 6);
        }
        M5.update();
    }
    MyMenu.show();
}
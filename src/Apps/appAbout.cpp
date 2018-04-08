#include "../apps.h"

void appAbout()
{
    MyMenu.drawAppMenu(F("ABOUT"), F(""), F("ESC"), F(""));

    while (M5.BtnB.wasPressed())
    {
        M5.update();
    }
    MyMenu.windowClr();
    M5.Lcd.drawCentreString(F("Made by"), M5.Lcd.width() / 2, (M5.Lcd.height() / 2) - 50, 4);
    M5.Lcd.drawCentreString(F("Botofan Calin"), M5.Lcd.width() / 2, (M5.Lcd.height() / 2) - 15, 4);
    M5.Lcd.drawCentreString(F("botofancalin@gmail.com"), M5.Lcd.width() / 2, (M5.Lcd.height() / 2) + 20, 4);
    while (!M5.BtnB.wasPressed())
    {
        M5.update();
    }
    MyMenu.show();
}
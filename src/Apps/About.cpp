#include "About.h"

void AboutClass::Run()
{
    M5m.windowClr();
    M5m.Lcd.drawCentreString(F("Made by"), M5m.Lcd.width() / 2, (M5m.Lcd.height() / 2) - 50, 4);
    M5m.Lcd.drawCentreString(F("Botofan Calin"), M5m.Lcd.width() / 2, (M5m.Lcd.height() / 2) - 15, 4);
    M5m.Lcd.drawCentreString(F("botofancalin@gmail.com"), M5m.Lcd.width() / 2, (M5m.Lcd.height() / 2) + 20, 4);
    while (!M5m.BtnB.wasPressed())
    {
        M5m.update();
    }
}

AboutClass::AboutClass()
{
    M5m.drawAppMenu(F("ABOUT"), F(""), F("ESC"), F(""));
    M5m.update();
}

AboutClass::~AboutClass()
{
    M5m.show();
}
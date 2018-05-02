#include "I2CScanner.h"

void I2CScannerClass::Run()
{
    M5m.update();
    M5m.drawAppMenu(F("I2C SCANNER"), F("ESC"), F("SCAN"), F(""));

    while (!M5m.BtnA.wasPressed())
    {
        if (scanrun == HIGH)
        {
            scanrun = LOW;
            nDevices = 0;
            for (address = 1; address < 127; address++)
            {
                ridx++;
                if (ridx == 17)
                {
                    ridx = 1;
                    lidx++;
                }
                Wire.beginTransmission(address);
                error = Wire.endTransmission();
                if (error == 0)
                {
                    M5m.Lcd.drawString(String(address, HEX), 0 + (ridx * 18), 45 + (lidx * 20), 2);
                    nDevices++;
                }
                else if (error == 4)
                {
                    M5m.Lcd.drawString(F("ER"), 0 + (ridx * 18), 45 + (lidx * 20), 2);
                }
                else
                {
                    M5m.Lcd.drawString(F("--"), 0 + (ridx * 18), 45 + (lidx * 20), 2);
                }
            }
            M5m.update();
        }
        else
        {
            if (M5m.BtnB.wasPressed())
            {
                M5m.windowClr();
                ridx = 0;
                lidx = 0;
                scanrun = HIGH;
            }
            M5m.update();
        }
    }
}

I2CScannerClass::I2CScannerClass()
{
}

I2CScannerClass::~I2CScannerClass()
{
    M5m.Lcd.setRotation(0);
                M5m.Lcd.fillScreen(0);
                M5m.drawAppMenu(F("TOOLS"), F("ESC"), F("SELECT"), F("LIST"));
                M5m.showList();
}
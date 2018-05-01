#include "I2CScanner.h"

void I2CScannerClass::Run()
{
    M5.update();
    M5.drawAppMenu(F("I2C SCANNER"), F("ESC"), F("SCAN"), F(""));

    while (!M5.BtnA.wasPressed())
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
                    M5.Lcd.drawString(String(address, HEX), 0 + (ridx * 18), 45 + (lidx * 20), 2);
                    nDevices++;
                }
                else if (error == 4)
                {
                    M5.Lcd.drawString(F("ER"), 0 + (ridx * 18), 45 + (lidx * 20), 2);
                }
                else
                {
                    M5.Lcd.drawString(F("--"), 0 + (ridx * 18), 45 + (lidx * 20), 2);
                }
            }
            M5.update();
        }
        else
        {
            if (M5.BtnB.wasPressed())
            {
                M5.windowClr();
                ridx = 0;
                lidx = 0;
                scanrun = HIGH;
            }
            M5.update();
        }
    }
}

I2CScannerClass::I2CScannerClass()
{
}

I2CScannerClass::~I2CScannerClass()
{
    M5.Lcd.setRotation(0);
                M5.Lcd.fillScreen(0);
                M5.drawAppMenu(F("TOOLS"), F("ESC"), F("SELECT"), F("LIST"));
                M5.showList();
}
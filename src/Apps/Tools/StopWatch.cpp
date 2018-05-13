#include "StopWatch.h"

void StopWatchClass::Run()
{
    M5m.update();
    M5m.drawAppMenu(F("STOPWATCH"), F("S/S"), F("ESC"), F("RES"));

    M5m.Lcd.drawString(F("MIN"), 40, 120, 2);
    M5m.Lcd.drawString(F("SEC"), 170, 120, 2);
    M5m.Lcd.drawFloat(tmp_sec, 1, 210, 100, 6);
    M5m.Lcd.drawNumber(tmp_min, 80, 100, 6);

    while (!M5m.BtnB.wasPressed())
    {
        M5m.update();
        if (M5m.BtnC.wasPressed())
        {
            tmp_sec = 0;
            tmp_min = 0;
            M5m.windowClr();
            M5m.Lcd.drawString(F("MIN"), 40, 120, 2);
            M5m.Lcd.drawString(F("SEC"), 170, 120, 2);
            M5m.Lcd.drawFloat(tmp_sec, 1, 210, 100, 6);
            M5m.Lcd.drawNumber(tmp_min, 80, 100, 6);
        }
        if (tmp_run)
        {
            if (M5m.BtnA.wasPressed())
            {
                tmp_run = LOW;
            }
            if (millis() - tmp_tmr > 100)
            {
                tmp_tmr = millis();
                tmp_sec = tmp_sec + 0.1;
                if (tmp_sec > 59.9)
                {
                    tmp_sec = 0;
                    tmp_min++;
                    M5m.windowClr();
                    M5m.Lcd.drawString(F("MIN"), 40, 120, 2);
                    M5m.Lcd.drawString(F("SEC"), 170, 120, 2);
                    M5m.Lcd.drawFloat(tmp_sec, 1, 210, 100, 6);
                    M5m.Lcd.drawNumber(tmp_min, 80, 100, 6);
                }
                M5m.Lcd.drawFloat(tmp_sec, 1, 210, 100, 6);
                M5m.Lcd.drawNumber(tmp_min, 80, 100, 6);
            }
        }
        else
        {
            if (M5m.BtnA.wasPressed())
            {
                tmp_run = HIGH;
            }
        }
    }
}

StopWatchClass::StopWatchClass()
{
}

StopWatchClass::~StopWatchClass()
{
    M5m.Lcd.fillScreen(0);
    M5m.drawAppMenu(F("TOOLS"), F("ESC"), F("SELECT"), F("LIST"));
    M5m.showList();
}
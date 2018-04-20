#include "../apps.h"

void appListTools()
{
    MyMenu.drawAppMenu(F("TOOLS"), F("ESC"), F("SELECT"), F("LIST"));

    while (M5.BtnB.wasPressed())
    {
        M5.update();
    }
    MyMenu.clearList();
    // add the your app menu list entry here
    MyMenu.setListCaption("TOOLS");
    MyMenu.addList("WIFI PACKET MONITOR");
    MyMenu.addList("WIFI SCANNER");
    MyMenu.addList("I2C SCANNER");
    MyMenu.addList("DHT TEMP & HUMIDITY");
    MyMenu.addList("STOPWATCH");
    MyMenu.addList("VOLTMETER");
    MyMenu.showList();

    while (!M5.BtnA.wasPressed())
    {
        if (M5.BtnC.wasPressed())
        {
            MyMenu.nextList();
        }
        if (M5.BtnB.wasPressed())
        {
            //The list items functions
            if (MyMenu.getListString() == "DHT TEMP & HUMIDITY")
            {
                M5.update();
                // The function to run
                Dht_Run();
                M5.Lcd.setRotation(0);
                M5.Lcd.fillScreen(0);
                MyMenu.drawAppMenu(F("TOOLS"), F("ESC"), F("SELECT"), F("LIST"));
                MyMenu.showList();
            }
            if (MyMenu.getListString() == "WIFI SCANNER")
            {
                M5.update();
                WiFiScanner_run();
                M5.Lcd.setRotation(0);
                M5.Lcd.fillScreen(0);
                MyMenu.drawAppMenu(F("TOOLS"), F("ESC"), F("SELECT"), F("LIST"));
                MyMenu.showList();
            }
            if (MyMenu.getListString() == "I2C SCANNER")
            {
                M5.update();
                while (!M5.BtnA.wasPressed())
                {
                    appI2CScanner();
                }
                M5.Lcd.setRotation(0);
                M5.Lcd.fillScreen(0);
                MyMenu.drawAppMenu(F("TOOLS"), F("ESC"), F("SELECT"), F("LIST"));
                MyMenu.showList();
            }
            if (MyMenu.getListString() == "WIFI PACKET MONITOR")
            {
                M5.update();
                Monitor_run();
                M5.Lcd.setRotation(0);
                M5.Lcd.fillScreen(0);
                MyMenu.drawAppMenu(F("TOOLS"), F("ESC"), F("SELECT"), F("LIST"));
                MyMenu.showList();
            }
            if (MyMenu.getListString() == "STOPWATCH")
            {
                M5.update();
                appStopWatch();
                M5.Lcd.setRotation(0);
                M5.Lcd.fillScreen(0);
                MyMenu.drawAppMenu(F("TOOLS"), F("ESC"), F("SELECT"), F("LIST"));
                MyMenu.showList();
            }
            if (MyMenu.getListString() == "VOLTMETER")
            {
                meters_setup();
                M5.update();
                while (!M5.BtnA.wasPressed())
                {
                    meters_run();
                }
                M5.Lcd.setRotation(0);
                M5.Lcd.fillScreen(0);
                MyMenu.drawAppMenu(F("TOOLS"), F("ESC"), F("SELECT"), F("LIST"));
                MyMenu.showList();
            }
        }
        M5.update();
    }
    MyMenu.show();
}
#include "../apps.h"

void appListTools()
{
    MyMenu.drawAppMenu(F("TOOLS"), F("ESC"), F("SELECT"), F("LIST"));

    while (M5.BtnB.wasPressed())
    {
        M5.update();
    }
    // add the your app menu list entry here
    MyMenu.clearList();
    MyMenu.setListCaption("TOOLS");
    MyMenu.addList("WIFI PACKET MONITOR");
    MyMenu.addList("WIFI SCANNER");
    MyMenu.addList("I2C SCANNER");
    MyMenu.addList("METERS");
    MyMenu.addList("DHT TEMP & HUMIDITY");
    MyMenu.addList("CLOCK");
    MyMenu.showList();

    while (!M5.BtnA.wasPressed())
    {
        if (M5.BtnC.wasPressed())
        {
            MyMenu.nextList();
        }
        if (M5.BtnB.wasPressed())
        {   // integrated app calling system
            if (MyMenu.getListString() == "CLOCK")
            {
                clock_setup(); // this is similar to Arduino "Setup" function
                M5.update();
                /*while BtnA is not pressed, the app is running. 
                Once BtnA was pressed, the app will exit
                Make sure you call M5.update(); into your app "Loop" like function*/
                while (!M5.BtnA.wasPressed())
                {
                    clock_run();// This is similar to Arduino "Loop" function
                }
                M5.Lcd.setRotation(0);
                M5.Lcd.fillScreen(0);
                MyMenu.drawAppMenu(F("TOOLS"), F("ESC"), F("SELECT"), F("LIST"));
                MyMenu.showList();
            }
            if (MyMenu.getListString() == "WIFI SCANNER")
            {
                M5.update();
                while (!M5.BtnA.wasPressed())
                {
                    WiFiScanner_run();
                }
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
                while (!M5.BtnB.wasPressed())
                {
                    Monitor_run();
                }
                M5.Lcd.setRotation(0);
                M5.Lcd.fillScreen(0);
                MyMenu.drawAppMenu(F("TOOLS"), F("ESC"), F("SELECT"), F("LIST"));
                MyMenu.showList();
            }
            if (MyMenu.getListString() == "METERS")
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
            if (MyMenu.getListString() == "DHT TEMP & HUMIDITY")
            {
                M5.update();
                Dht_Run();
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
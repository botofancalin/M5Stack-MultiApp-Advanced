#include "../apps.h"

unsigned long counter;

void appWiFiSetup()
{
    MyMenu.drawAppMenu(F("WiFi"), F("ESC"), F("SELECT"), F("LIST"));

    while (M5.BtnB.wasPressed())
    {
        M5.update();
    }

    MyMenu.clearList();
    MyMenu.setListCaption("WiFi");
    MyMenu.addList("WPS Button");
    MyMenu.addList("WPS Pin Code");
    MyMenu.showList();

    while (!M5.BtnA.wasPressed())
    {
        if (M5.BtnC.wasPressed())
        {
            MyMenu.nextList();
        }

        if (M5.BtnB.wasPressed())
        {
            if (MyMenu.getListString() == "WPS Button")
            {
                MyMenu.windowClr();
                delay(200);
                Wps_run(1);
                while (!WiFi.isConnected())
                {
                    delay(50);
                    counter++;
                    if (counter > 1200)
                    {
                        MyMenu.windowClr();
                        M5.update();
                        break;
                    }
                }
                MyMenu.windowClr();
                MyMenu.drawAppMenu(F("WPS Mode"), F("ESC"), F("SELECT"), F("LIST"));
                MyMenu.showList();
            }
            if (MyMenu.getListString() == "WPS Pin Code")
            {
                MyMenu.windowClr();
                delay(200);
                Wps_run(2);
                while (!WiFi.isConnected())
                {
                    delay(50);
                    counter++;
                    if (counter > 2400)
                    {
                        MyMenu.windowClr();
                        M5.update();
                        break;
                    }
                }
                MyMenu.windowClr();
                MyMenu.drawAppMenu(F("WPS Mode"), F("ESC"), F("SELECT"), F("LIST"));
                MyMenu.showList();
            }
        }
        M5.update();
    }
    MyMenu.show();
}
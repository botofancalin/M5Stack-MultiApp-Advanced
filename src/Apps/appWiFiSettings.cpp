#include "../apps.h"

void AP_Mode()
{
    WiFi.disconnect();
    vTaskDelay(200 / portTICK_PERIOD_MS);
    WiFi.mode(WIFI_MODE_AP);
    WiFi_Mode = WIFI_MODE_AP;
    WiFi.begin("M5Stack");
    WiFi.softAPsetHostname("M5Stack");
    M5.Lcd.drawString("AP Mode Started", 5, 50, 2);
    M5.Lcd.drawString("Host Name: M5Stack", 5, 70, 2);
    M5.Lcd.drawString("IP Address: " + WiFi.softAPIP().toString(), 5, 90, 2);
}

void STA_Mode()
{
    WiFi.disconnect();
    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin();
    WiFi_Mode = WIFI_MODE_STA;
    M5.Lcd.drawString("STA Mode Started", 5, 50, 2);
    M5.Lcd.drawString("Will Connect to sored SSID", 5, 70, 2);
}

void APSTA_Mode()
{
    WiFi.disconnect();
    WiFi.mode(WIFI_MODE_APSTA);
    WiFi.begin();
    WiFi_Mode = WIFI_MODE_APSTA;
    M5.Lcd.drawString("AP + STA Mode Started", 5, 50, 2);
    M5.Lcd.drawString("Will use the sored SSID", 5, 70, 2);
}

void appWiFiSetup()
{
    MyMenu.drawAppMenu(F("WiFi"), F("ESC"), F("SELECT"), F("LIST"));

    while (M5.BtnB.wasPressed())
    {
        M5.update();
    }

    MyMenu.clearList();
    MyMenu.setListCaption("WiFi");
    MyMenu.addList("Connect by WPS Button");
    MyMenu.addList("Connect by WPS Pin Code");
    MyMenu.addList("WiFi STA");
    MyMenu.addList("WiFi AP");
    MyMenu.addList("WiFi AP + STA");
    MyMenu.addList("WiFi OFF");
    MyMenu.showList();

    while (!M5.BtnA.wasPressed())
    {
        if (M5.BtnC.wasPressed())
        {
            MyMenu.nextList();
        }
        if (M5.BtnB.wasPressed())
        {
            if (MyMenu.getListString() == "WiFi STA")
            {
                MyMenu.windowClr();
                vTaskDelay(200 / portTICK_PERIOD_MS);
                STA_Mode();
                while (!M5.BtnA.wasPressed())
                {
                    M5.update();
                }
                MyMenu.windowClr();
                MyMenu.drawAppMenu(F("WiFi"), F("ESC"), F("SELECT"), F("LIST"));
                MyMenu.showList();
            }
            if (MyMenu.getListString() == "WiFi AP + STA")
            {
                MyMenu.windowClr();
                vTaskDelay(200 / portTICK_PERIOD_MS);
                APSTA_Mode();
                while (!M5.BtnA.wasPressed())
                {
                    M5.update();
                }
                MyMenu.windowClr();
                MyMenu.drawAppMenu(F("WiFi"), F("ESC"), F("SELECT"), F("LIST"));
                MyMenu.showList();
            }
            if (MyMenu.getListString() == "WiFi AP")
            {
                MyMenu.windowClr();
                vTaskDelay(200 / portTICK_PERIOD_MS);
                AP_Mode();
                while (!M5.BtnA.wasPressed())
                {
                    M5.update();
                }
                MyMenu.windowClr();
                MyMenu.drawAppMenu(F("WiFi"), F("ESC"), F("SELECT"), F("LIST"));
                MyMenu.showList();
            }
            if (MyMenu.getListString() == "Connect by WPS Button")
            {
                MyMenu.windowClr();
                vTaskDelay(200 / portTICK_PERIOD_MS);
                Wps_run(true);
                WiFi_Mode = WIFI_MODE_STA;
            }
            if (MyMenu.getListString() == "Connect by WPS Pin Code")
            {
                MyMenu.windowClr();
                vTaskDelay(200 / portTICK_PERIOD_MS);
                Wps_run(false);
                WiFi_Mode = WIFI_MODE_STA;
            }
            if (MyMenu.getListString() == "WiFi OFF")
            {
                MyMenu.windowClr();
                vTaskDelay(200 / portTICK_PERIOD_MS);
                WiFi.disconnect();
                WiFi.mode(WIFI_MODE_NULL);
                WiFi_Mode = WIFI_MODE_NULL;
                M5.Lcd.drawString("WiFi Turned OFF", 5, 50, 2);
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                MyMenu.drawAppMenu(F("WiFi"), F("ESC"), F("SELECT"), F("LIST"));
                MyMenu.showList();
            }
        }
        M5.update();
    }
    preferences.begin("WiFi-mode", false);
    preferences.putInt("mode", WiFi_Mode);
    preferences.end();
    MyMenu.show();
}
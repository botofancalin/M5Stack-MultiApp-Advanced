#include "WiFiSettings.h"

void WifiSettingsClass::AP_Mode()
{
    WiFi.disconnect();
    vTaskDelay(200 / portTICK_PERIOD_MS);
    WiFi.mode(WIFI_MODE_AP);
    WiFi_Mode = WIFI_MODE_AP;
    WiFi.begin("M5Stack");
    WiFi.softAPsetHostname("M5Stack");
    M5m.Lcd.drawString("AP Mode Started", 5, 50, 2);
    M5m.Lcd.drawString("Host Name: M5Stack", 5, 70, 2);
    M5m.Lcd.drawString("IP Address: " + WiFi.softAPIP().toString(), 5, 90, 2);
}

void WifiSettingsClass::STA_Mode()
{
    WiFi.disconnect();
    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin();
    WiFi_Mode = WIFI_MODE_STA;
    M5m.Lcd.drawString("STA Mode Started", 5, 50, 2);
    M5m.Lcd.drawString("Will Connect to sored SSID", 5, 70, 2);
}

void WifiSettingsClass::APSTA_Mode()
{
    WiFi.disconnect();
    WiFi.mode(WIFI_MODE_APSTA);
    WiFi.begin();
    WiFi_Mode = WIFI_MODE_APSTA;
    M5m.Lcd.drawString("AP + STA Mode Started", 5, 50, 2);
    M5m.Lcd.drawString("Will use the sored SSID", 5, 70, 2);
}

void WifiSettingsClass::SmartConfig()
{
    int i = 0;
    WiFi.mode(WIFI_AP_STA);
    WiFi.beginSmartConfig();
    M5m.Lcd.drawString("Waiting for SmartConfig", 5, 30, 2);
    while (!WiFi.smartConfigDone())
    {
        M5m.Lcd.setTextColor(WHITE);
        M5m.Lcd.drawNumber(i, 5, 50, 2);
        delay(500);
        M5m.Lcd.setTextColor(BLACK);
        M5m.Lcd.drawNumber(i, 5, 50, 2);
        if (i == 119)
        {
            M5m.Lcd.setTextColor(WHITE);
            M5m.Lcd.drawString("SmartConfig NOT received!", 5, 70, 2);
            STA_Mode();
            return;
        }
        i++;
    }
    M5m.Lcd.setTextColor(WHITE);
    M5m.Lcd.drawString("SmartConfig received", 5, 70, 2);
    M5m.Lcd.drawString("Waiting for WiFi", 5, 90, 2);
    i = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        M5m.Lcd.setTextColor(WHITE);
        M5m.Lcd.drawNumber(i, 5, 110, 2);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        M5m.Lcd.setTextColor(BLACK);
        M5m.Lcd.drawNumber(i, 5, 110, 2);
        if (i == 59)
        {
            STA_Mode();
            M5m.Lcd.setTextColor(WHITE);
            M5m.Lcd.drawString("Wifi Not Found!", 5, 130, 2);
            return;
        }
        i++;
    }
    M5m.Lcd.setTextColor(WHITE);
    M5m.Lcd.drawString("WiFi Connected", 5, 130, 2);
    M5m.Lcd.drawString("IP: " + WiFi.localIP().toString(), 5, 150, 2);
    WiFi_Mode = WIFI_MODE_STA;
}

void WifiSettingsClass::Run()
{
    M5m.clearList();
    M5m.setListCaption("WiFi");
    M5m.addList("WiFi SmartConfig");
    M5m.addList("Connect by WPS Button");
    M5m.addList("Connect by WPS Pin Code");
    M5m.addList("WiFi STA");
    M5m.addList("WiFi AP");
    M5m.addList("WiFi OFF");
    M5m.showList();

    while (!M5m.BtnA.wasPressed())
    {
        if (M5m.BtnC.wasPressed())
        {
            M5m.nextList();
        }
        if (M5m.BtnB.wasPressed())
        {
            if (M5m.getListString() == "WiFi STA")
            {
                M5m.windowClr();
                STA_Mode();
                vTaskDelay(2000 / portTICK_PERIOD_MS);
                M5m.windowClr();
                M5m.drawAppMenu(F("WiFi"), F("ESC"), F("SELECT"), F("LIST"));
                M5m.showList();
            }
            if (M5m.getListString() == "WiFi SmartConfig")
            {
                M5m.windowClr();
                SmartConfig();
                vTaskDelay(2000 / portTICK_PERIOD_MS);
                M5m.windowClr();
                M5m.drawAppMenu(F("WiFi"), F("ESC"), F("SELECT"), F("LIST"));
                M5m.showList();
            }
            if (M5m.getListString() == "WiFi AP")
            {
                M5m.windowClr();
                AP_Mode();
                vTaskDelay(2000 / portTICK_PERIOD_MS);
                M5m.windowClr();
                M5m.drawAppMenu(F("WiFi"), F("ESC"), F("SELECT"), F("LIST"));
                M5m.showList();
            }
            if (M5m.getListString() == "Connect by WPS Button")
            {
                M5m.windowClr();
                vTaskDelay(200 / portTICK_PERIOD_MS);
                Wps_run(true);
                WiFi_Mode = WIFI_MODE_STA;
            }
            if (M5m.getListString() == "Connect by WPS Pin Code")
            {
                M5m.windowClr();
                vTaskDelay(200 / portTICK_PERIOD_MS);
                Wps_run(false);
                WiFi_Mode = WIFI_MODE_STA;
            }
            if (M5m.getListString() == "WiFi OFF")
            {
                M5m.windowClr();
                vTaskDelay(200 / portTICK_PERIOD_MS);
                WiFi.disconnect();
                WiFi.mode(WIFI_MODE_NULL);
                WiFi_Mode = WIFI_MODE_NULL;
                M5m.Lcd.drawString("WiFi Turned OFF", 5, 50, 2);
                vTaskDelay(2000 / portTICK_PERIOD_MS);
                M5m.drawAppMenu(F("WiFi"), F("ESC"), F("SELECT"), F("LIST"));
                M5m.showList();
            }
        }
        M5m.update();
    }
    preferences.begin("WiFi", false);
    preferences.putInt("mode", (int)WiFi.getMode());
    preferences.end();
}

WifiSettingsClass::WifiSettingsClass()
{
    M5m.drawAppMenu(F("WiFi"), F("ESC"), F("SELECT"), F("LIST"));
    M5m.update();
}

WifiSettingsClass::~WifiSettingsClass()
{
    M5m.show();
}
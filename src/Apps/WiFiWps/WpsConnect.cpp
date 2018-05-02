
extern "C" {
#include "esp_wifi.h"
#include "esp_wps.h"
}

#include "WpsConnect.h"

esp_wps_config_t config;

String wpspin2string(uint8_t a[])
{
    char wps_pin[9];
    for (int i = 0; i < 8; i++)
    {
        wps_pin[i] = a[i];
    }
    wps_pin[8] = '\0';
    return (String)wps_pin;
}

void WiFiEvent(WiFiEvent_t event, system_event_info_t info)
{
    switch (event)
    {
    case SYSTEM_EVENT_STA_START:
        M5m.Lcd.drawString("Station Mode Started", 5, 50, 2);
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        M5m.Lcd.drawString("Connected to :" + String(WiFi.SSID()), 5, 110, 2);
        M5m.Lcd.drawString("IP: " + WiFi.localIP().toString(), 5, 130, 2);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        M5m.Lcd.drawString("Reconnecting to Station", 5, 70, 2);
        WiFi.reconnect();
        break;
    case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
        M5m.Lcd.drawString("WPS Successfull: " + String(WiFi.SSID()), 5, 90, 2);
        esp_wifi_wps_disable();
        delay(10);
        WiFi.begin();
        break;
    case SYSTEM_EVENT_STA_WPS_ER_FAILED:
        M5m.Lcd.drawString("WPS Failed, retrying", 5, 70, 2);
        esp_wifi_wps_disable();
        esp_wifi_wps_enable(&config);
        esp_wifi_wps_start(0);
        break;
    case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
        M5m.Lcd.drawString("WPS Timedout, retrying", 5, 70, 2);
        esp_wifi_wps_disable();
        esp_wifi_wps_enable(&config);
        esp_wifi_wps_start(0);
        break;
    case SYSTEM_EVENT_STA_WPS_ER_PIN:
        M5m.Lcd.drawString("WPS PIN = " + wpspin2string(info.sta_er_pin.pin_code), 5, 170, 4);
        break;
    default:
        break;
    }
}

void Wps_run(bool mode)
{
    WiFi.disconnect();
    M5m.Lcd.drawString("Starting WPS", 5, 30, 2);
    if (mode)
    {
        config = WPS_CONFIG_INIT_DEFAULT(WPS_TYPE_PBC);
    }
    else
    {
        config = WPS_CONFIG_INIT_DEFAULT(WPS_TYPE_PIN);
    }

    WiFi.onEvent(WiFiEvent);
    WiFi.mode(WIFI_MODE_STA);
    esp_wifi_wps_enable(&config);
    esp_wifi_wps_start(0);
}
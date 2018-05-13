#include "WifiScanner.h"

void WifiScannerClass::Run()
{
    M5m.update();
    M5m.drawAppMenu(F("WiFi SCANNER"), F("ESC"), F("SCAN"), F("PAGE"));
    M5m.Lcd.drawCentreString(F("SCANNING....."), M5m.Lcd.width() / 2, M5m.Lcd.height() / 2, 2);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    WiFi.mode(WIFI_MODE_STA);
    WiFi.disconnect();
    vTaskDelay(100 / portTICK_PERIOD_MS);

    while (!M5m.BtnA.wasPressed())
    {
        while (M5m.BtnB.wasPressed())
        {
            M5m.update();
        }

        wifi_count = WiFi.scanNetworks();

        if (wifi_count > 0)
        {
            if (wifi_count > list_lines)
            {
                list_lastpagelines = wifi_count % list_lines;
                if (list_lastpagelines > 0)
                {
                    list_pages = (wifi_count - list_lastpagelines) / list_lines;
                    list_pages++;
                }
                else
                {
                    list_pages = wifi_count / list_lines;
                }
            }
            else
            {
                list_pages = 1;
            }
        }
        while (!M5m.BtnA.wasPressed())
        {
            if (wifi_count == 0)
            {
                if (!wifi_showlock)
                {
                    M5m.windowClr();
                    M5m.Lcd.drawCentreString(F("NO NETWORKS FOUND"), M5m.Lcd.width() / 2, M5m.Lcd.height() / 2, 2);
                    wifi_showlock = HIGH;
                }
            }
            else
            {
                if (!wifi_showlock)
                {
                    M5m.windowClr();
                    M5m.Lcd.drawCentreString("FOUND " + String(wifi_count) + " NETWORKS, PAGE: " + String(list_page + 1) + "/" + String(list_pages), M5m.Lcd.width() / 2, 40, 2);
                    if ((list_page + 1) == list_pages)
                    {
                        if (list_lastpagelines == 0 and wifi_count >= list_lines)
                        {
                            for (short i = 0; i < list_lines; i++)
                            {
                                M5m.Lcd.drawString(WiFi.SSID(i + (list_page * list_lines)), 5, 80 + (i * 20), 2);
                                M5m.Lcd.drawString("ch " + String(WiFi.channel(i + (list_page * list_lines))), 200, 80 + (i * 20), 2);
                                M5m.Lcd.drawString(String(WiFi.RSSI(i + (list_page * list_lines))) + " dB", 150, 80 + (i * 20), 2);
                                switch ((WiFi.encryptionType(i + (list_page * list_lines))))
                                {
                                case WIFI_AUTH_OPEN:
                                    Encryption = "OPEN";
                                    break;
                                case WIFI_AUTH_WEP:
                                    Encryption = "WEP";
                                    break;
                                case WIFI_AUTH_WPA_PSK:
                                    Encryption = "WPA PSK";
                                    break;
                                case WIFI_AUTH_WPA2_PSK:
                                    Encryption = "WPA2 PSK";
                                    break;
                                case WIFI_AUTH_WPA_WPA2_PSK:
                                    Encryption = "WPA WPA2 PSK";
                                    break;
                                case WIFI_AUTH_WPA2_ENTERPRISE:
                                    Encryption = "WPA2 ENTERPRISE";
                                    break;
                                case WIFI_AUTH_MAX:
                                    Encryption = "AUTH MAX";
                                    break;
                                default:
                                    Encryption = "Unknown";
                                    break;
                                }
                                M5m.Lcd.drawString(Encryption, 200, 80 + (i * 20), 2);
                            }
                        }
                        else
                        {
                            if (list_pages > 1)
                            {
                                for (short i = 0; i < list_lastpagelines; i++)
                                {
                                    M5m.Lcd.drawString(WiFi.SSID(i + (list_page * list_lines)), 5, 80 + (i * 20), 2);
                                    M5m.Lcd.drawString("ch " + String(WiFi.channel(i + (list_page * list_lines))), 200, 80 + (i * 20), 2);
                                    M5m.Lcd.drawString(String(WiFi.RSSI(i + (list_page * list_lines))) + " dB", 150, 80 + (i * 20), 2);
                                    switch ((WiFi.encryptionType(i + (list_page * list_lines))))
                                    {
                                    case WIFI_AUTH_OPEN:
                                        Encryption = "OPEN";
                                        break;
                                    case WIFI_AUTH_WEP:
                                        Encryption = "WEP";
                                        break;
                                    case WIFI_AUTH_WPA_PSK:
                                        Encryption = "WPA PSK";
                                        break;
                                    case WIFI_AUTH_WPA2_PSK:
                                        Encryption = "WPA2 PSK";
                                        break;
                                    case WIFI_AUTH_WPA_WPA2_PSK:
                                        Encryption = "WPA WPA2";
                                        break;
                                    case WIFI_AUTH_WPA2_ENTERPRISE:
                                        Encryption = "WPA2 ENT";
                                        break;
                                    case WIFI_AUTH_MAX:
                                        Encryption = "AUTH MAX";
                                        break;
                                    default:
                                        Encryption = "Unknown";
                                        break;
                                    }
                                    M5m.Lcd.drawString(Encryption, 250, 80 + (i * 20), 2);
                                }
                            }
                            else
                            {
                                for (short i = 0; i < wifi_count; i++)
                                {
                                    M5m.Lcd.drawString(WiFi.SSID(i + (list_page * list_lines)), 5, 80 + (i * 20), 2);
                                    M5m.Lcd.drawString("ch " + String(WiFi.channel(i + (list_page * list_lines))), 200, 80 + (i * 20), 2);
                                    M5m.Lcd.drawString(String(WiFi.RSSI(i + (list_page * list_lines))) + " dB", 150, 80 + (i * 20), 2);
                                    switch ((WiFi.encryptionType(i + (list_page * list_lines))))
                                    {
                                    case WIFI_AUTH_OPEN:
                                        Encryption = "OPEN";
                                        break;
                                    case WIFI_AUTH_WEP:
                                        Encryption = "WEP";
                                        break;
                                    case WIFI_AUTH_WPA_PSK:
                                        Encryption = "WPA PSK";
                                        break;
                                    case WIFI_AUTH_WPA2_PSK:
                                        Encryption = "WPA2 PSK";
                                        break;
                                    case WIFI_AUTH_WPA_WPA2_PSK:
                                        Encryption = "WPA WPA2 PSK";
                                        break;
                                    case WIFI_AUTH_WPA2_ENTERPRISE:
                                        Encryption = "WPA2 ENTERPRISE";
                                        break;
                                    case WIFI_AUTH_MAX:
                                        Encryption = "AUTH MAX";
                                        break;
                                    default:
                                        Encryption = "Unknown";
                                        break;
                                    }
                                    M5m.Lcd.drawString(Encryption, 250, 80 + (i * 20), 2);
                                }
                            }
                        }
                    }
                    else
                    {
                        for (short i = 0; i < list_lines; i++)
                        {
                            M5m.Lcd.drawString(WiFi.SSID(i + (list_page * list_lines)), 5, 80 + (i * 20), 2);
                            M5m.Lcd.drawString("ch " + String(WiFi.channel(i + (list_page * list_lines))), 200, 80 + (i * 20), 2);
                            M5m.Lcd.drawString(String(WiFi.RSSI(i + (list_page * list_lines))) + " dB", 150, 80 + (i * 20), 2);
                            switch ((WiFi.encryptionType(i + (list_page * list_lines))))
                            {
                            case WIFI_AUTH_OPEN:
                                Encryption = "OPEN";
                                break;
                            case WIFI_AUTH_WEP:
                                Encryption = "WEP";
                                break;
                            case WIFI_AUTH_WPA_PSK:
                                Encryption = "WPA PSK";
                                break;
                            case WIFI_AUTH_WPA2_PSK:
                                Encryption = "WPA2 PSK";
                                break;
                            case WIFI_AUTH_WPA_WPA2_PSK:
                                Encryption = "WPA WPA2 PSK";
                                break;
                            case WIFI_AUTH_WPA2_ENTERPRISE:
                                Encryption = "WPA2 ENTERPRISE";
                                break;
                            case WIFI_AUTH_MAX:
                                Encryption = "AUTH MAX";
                                break;
                            default:
                                Encryption = "Unknown";
                                break;
                            }
                            M5m.Lcd.drawString(Encryption, 250, 80 + (i * 20), 2);
                        }
                    }
                    wifi_showlock = HIGH;
                }
            }
            if (M5m.BtnB.wasPressed())
            {
                list_page = 0;
                list_pages = 0;
                M5m.windowClr();
                M5m.Lcd.drawCentreString(F("SCANNING DEEPER....."), M5m.Lcd.width() / 2, M5m.Lcd.height() / 2, 2);
                wifi_count = WiFi.scanNetworks(false, true, false, 500);
                wifi_showlock = LOW;
                if (wifi_count > 0)
                {
                    if (wifi_count > list_lines)
                    {
                        list_lastpagelines = wifi_count % list_lines;
                        if (list_lastpagelines > 0)
                        {
                            list_pages = (wifi_count - list_lastpagelines) / list_lines;
                            list_pages++;
                        }
                        else
                        {
                            list_pages = wifi_count / list_lines;
                        }
                    }
                    else
                    {
                        list_pages = 1;
                    }
                }
            }
            if (M5m.BtnC.wasPressed())
            {
                if (wifi_count > 0 and list_pages > 1)
                {
                    list_page++;
                    if (list_page == list_pages)
                    {
                        list_page = 0;
                    }
                    wifi_showlock = LOW;
                }
            }
            M5m.update();
        }
    }
    preferences.begin("WiFi", false);
    WiFi_Mode = preferences.getInt("mode", 0);
    WiFi.mode(wifi_mode_t(WiFi_Mode));
    if (WiFi_Mode != 0)
    {
        WiFi.begin();
    }
    preferences.end();
    return;
}

WifiScannerClass::WifiScannerClass()
{
}

WifiScannerClass::~WifiScannerClass()
{
    M5m.Lcd.fillScreen(0);
    M5m.drawAppMenu(F("TOOLS"), F("ESC"), F("SELECT"), F("LIST"));
    M5m.showList();
}
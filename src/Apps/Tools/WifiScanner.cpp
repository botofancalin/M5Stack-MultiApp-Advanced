#include "../../apps.h"

void WiFiScanner_run()
{
    int wifi_count = 0;
    bool wifi_showlock = LOW;
    short list_lines = 5;
    int list_page = 0;
    int list_pages = 0;
    int list_lastpagelines = 0;
    String Encryption = "Unknown";

    MyMenu.drawAppMenu(F("WiFi SCANNER"), F("ESC"), F("SCAN"), F("PAGE"));

    M5.Lcd.drawCentreString(F("SCANNING....."), M5.Lcd.width() / 2, M5.Lcd.height() / 2, 2);

    WiFi.disconnect();
    delay(500);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(500);

    while (!M5.BtnA.wasPressed())
    {
        while (M5.BtnB.wasPressed())
        {
            M5.update();
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
        while (!M5.BtnA.wasPressed())
        {
            if (wifi_count == 0)
            {
                if (!wifi_showlock)
                {
                    MyMenu.windowClr();
                    M5.Lcd.drawCentreString(F("NO NETWORKS FOUND"), M5.Lcd.width() / 2, M5.Lcd.height() / 2, 2);
                    wifi_showlock = HIGH;
                }
            }
            else
            {
                if (!wifi_showlock)
                {
                    MyMenu.windowClr();
                    M5.Lcd.drawCentreString("FOUND " + String(wifi_count) + " NETWORKS, PAGE: " + String(list_page + 1) + "/" + String(list_pages), M5.Lcd.width() / 2, 40, 2);
                    if ((list_page + 1) == list_pages)
                    {
                        if (list_lastpagelines == 0 and wifi_count >= list_lines)
                        {
                            for (short i = 0; i < list_lines; i++)
                            {
                                M5.Lcd.drawString(WiFi.SSID(i + (list_page * list_lines)), 5, 80 + (i * 20), 2);
                                M5.Lcd.drawString("ch " + String(WiFi.channel(i + (list_page * list_lines))), 200, 80 + (i * 20), 2);
                                M5.Lcd.drawString(String(WiFi.RSSI(i + (list_page * list_lines))) + " dB", 150, 80 + (i * 20), 2);
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
                                M5.Lcd.drawString(Encryption, 200, 80 + (i * 20), 2);
                            }
                        }
                        else
                        {
                            if (list_pages > 1)
                            {
                                for (short i = 0; i < list_lastpagelines; i++)
                                {
                                    M5.Lcd.drawString(WiFi.SSID(i + (list_page * list_lines)), 5, 80 + (i * 20), 2);
                                    M5.Lcd.drawString("ch " + String(WiFi.channel(i + (list_page * list_lines))), 200, 80 + (i * 20), 2);
                                    M5.Lcd.drawString(String(WiFi.RSSI(i + (list_page * list_lines))) + " dB", 150, 80 + (i * 20), 2);
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
                                    M5.Lcd.drawString(Encryption, 250, 80 + (i * 20), 2);
                                }
                            }
                            else
                            {
                                for (short i = 0; i < wifi_count; i++)
                                {
                                    M5.Lcd.drawString(WiFi.SSID(i + (list_page * list_lines)), 5, 80 + (i * 20), 2);
                                    M5.Lcd.drawString("ch " + String(WiFi.channel(i + (list_page * list_lines))), 200, 80 + (i * 20), 2);
                                    M5.Lcd.drawString(String(WiFi.RSSI(i + (list_page * list_lines))) + " dB", 150, 80 + (i * 20), 2);
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
                                    M5.Lcd.drawString(Encryption, 250, 80 + (i * 20), 2);
                                }
                            }
                        }
                    }
                    else
                    {
                        for (short i = 0; i < list_lines; i++)
                        {
                            M5.Lcd.drawString(WiFi.SSID(i + (list_page * list_lines)), 5, 80 + (i * 20), 2);
                            M5.Lcd.drawString("ch " + String(WiFi.channel(i + (list_page * list_lines))), 200, 80 + (i * 20), 2);
                            M5.Lcd.drawString(String(WiFi.RSSI(i + (list_page * list_lines))) + " dB", 150, 80 + (i * 20), 2);
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
                            M5.Lcd.drawString(Encryption, 250, 80 + (i * 20), 2);
                        }
                    }
                    wifi_showlock = HIGH;
                }
            }
            if (M5.BtnB.wasPressed())
            {
                list_page = 0;
                list_pages = 0;
                MyMenu.windowClr();
                M5.Lcd.drawCentreString(F("SCANNING DEEPER....."), M5.Lcd.width() / 2, M5.Lcd.height() / 2, 2);
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
            if (M5.BtnC.wasPressed())
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
            M5.update();
        }
    }
    return;
}
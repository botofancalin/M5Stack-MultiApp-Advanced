#include "../apps.h"

String getWiFiMac()
{
    uint8_t baseMac[6];
    esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
    char baseMacChr[18] = {0};
    sprintf(baseMacChr, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
    return String(baseMacChr);
}

void appSysInfo()
{
    MyMenu.drawAppMenu(F("M5 SYSTEM INFO"), F(""), F("ESC"), F(""));

    int chipRev = ESP.getChipRevision();
    int cpuSpeed = ESP.getCpuFreqMHz();
    int flashSize = ESP.getFlashChipSize();
    int flashSpeed = ESP.getFlashChipSpeed();
    String WiFiMAC = getWiFiMac();
    int ramFree = esp_get_free_heap_size();

    while (M5.BtnB.wasPressed())
    {
        M5.update();
    }

    M5.Lcd.drawString(F("CPU FREQ:"), 10, 40, 2);
    M5.Lcd.drawNumber(cpuSpeed, 120, 40, 2);

     M5.Lcd.drawString(F("CPU CORES:"), 10, 60, 2);
    M5.Lcd.drawNumber(2, 120, 60, 2);

    M5.Lcd.drawString(F("CHIP REV:"), 10, 80, 2);
    M5.Lcd.drawNumber(chipRev, 120, 80, 2);

    M5.Lcd.drawString(F("FLASH SIZE:"), 10, 100, 2);
    M5.Lcd.drawNumber(flashSize, 120, 100, 2);

    M5.Lcd.drawString(F("FLASH SPEED:"), 10, 120, 2);
    M5.Lcd.drawNumber(flashSpeed, 120, 120, 2);

    M5.Lcd.drawString(F("FREE RAM:"), 10, 140, 2);
    M5.Lcd.drawNumber(ramFree, 120, 140, 2);

    M5.Lcd.drawString(F("WIFI STA MAC:"), 10, 160, 2);
    M5.Lcd.drawString(WiFiMAC, 120, 160, 2);

    M5.Lcd.drawString(F("IP ADDRESS:"), 10, 180, 2);
    M5.Lcd.drawString((WiFi.localIP().toString()), 120, 180, 2);

    while (!M5.BtnB.wasPressed())
    {
        M5.update();
    }

    MyMenu.show();
}
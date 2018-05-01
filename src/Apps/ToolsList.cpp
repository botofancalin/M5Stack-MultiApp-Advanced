#include "ToolsList.h"

void appListTools()
{
    M5.drawAppMenu(F("TOOLS"), F("ESC"), F("SELECT"), F("LIST"));

    while (M5.BtnB.wasPressed())
    {
        M5.update();
    }
    M5.clearList();
    M5.setListCaption("TOOLS");
    M5.addList("WIFI PACKET MONITOR");
    M5.addList("WIFI SCANNER");
    M5.addList("I2C SCANNER");
    M5.addList("DHT TEMP & HUMIDITY");
    M5.addList("STOPWATCH");
    M5.showList();

    while (!M5.BtnA.wasPressed())
    {
        if (M5.BtnC.wasPressed())
        {
            M5.nextList();
        }
        if (M5.BtnB.wasPressed())
        {
            if (M5.getListString() == "DHT TEMP & HUMIDITY")
            {
                DhtReaderClass DhtReaderObj;
                DhtReaderObj.Run();
            }
            if (M5.getListString() == "WIFI SCANNER")
            {
                WifiScannerClass WifiScannerObj;
                WifiScannerObj.Run();
            }
            if (M5.getListString() == "I2C SCANNER")
            {
               I2CScannerClass I2CScannerObj;
               I2CScannerObj.Run();
            }
            if (M5.getListString() == "WIFI PACKET MONITOR")
            {
               Monitor_run();
            }
            if (M5.getListString() == "STOPWATCH")
            {
                StopWatchClass StopWatchObj;
                StopWatchObj.Run();
            }
        }
        M5.update();
    }
    M5.show();
}
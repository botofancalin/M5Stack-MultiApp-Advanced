#include "ToolsList.h"

void appListTools()
{
    M5m.drawAppMenu(F("TOOLS"), F("ESC"), F("SELECT"), F("LIST"));

    while (M5m.BtnB.wasPressed())
    {
        M5m.update();
    }
    M5m.clearList();
    M5m.setListCaption("TOOLS");
    M5m.addList("FOXGRAPH");
    M5m.addList("WIFI PACKET MONITOR");
    M5m.addList("WIFI SCANNER");
    M5m.addList("I2C SCANNER");
    M5m.addList("DHT TEMP & HUMIDITY");
    M5m.addList("STOPWATCH");
    M5m.showList();

    while (!M5m.BtnA.wasPressed())
    {
        if (M5m.BtnC.wasPressed())
        {
            M5m.nextList();
        }
        if (M5m.BtnB.wasPressed())
        {
            if (M5m.getListString() == "DHT TEMP & HUMIDITY")
            {
                DhtReaderClass DhtReaderObj;
                DhtReaderObj.Run();
            }
            if (M5m.getListString() == "WIFI SCANNER")
            {
                WifiScannerClass WifiScannerObj;
                WifiScannerObj.Run();
            }
            if (M5m.getListString() == "I2C SCANNER")
            {
               I2CScannerClass I2CScannerObj;
               I2CScannerObj.Run();
            }
            if (M5m.getListString() == "WIFI PACKET MONITOR")
            {
               Monitor_run();
            }
            if (M5m.getListString() == "STOPWATCH")
            {
                StopWatchClass StopWatchObj;
                StopWatchObj.Run();
            }
            if (M5m.getListString() == "FOXGRAPH")
            {
                FoxGraphClass FoxGraphObj;
                FoxGraphObj.Run();
            }
        }
        M5m.update();
    }
    M5m.show();
}
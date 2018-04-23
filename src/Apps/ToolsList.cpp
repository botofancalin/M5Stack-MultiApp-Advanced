#include "ToolsList.h"

void appListTools()
{
    MyMenu.drawAppMenu(F("TOOLS"), F("ESC"), F("SELECT"), F("LIST"));

    while (M5.BtnB.wasPressed())
    {
        M5.update();
    }
    MyMenu.clearList();
    MyMenu.setListCaption("TOOLS");
    MyMenu.addList("WIFI PACKET MONITOR");
    MyMenu.addList("WIFI SCANNER");
    MyMenu.addList("I2C SCANNER");
    MyMenu.addList("DHT TEMP & HUMIDITY");
    MyMenu.addList("STOPWATCH");
    MyMenu.showList();

    while (!M5.BtnA.wasPressed())
    {
        if (M5.BtnC.wasPressed())
        {
            MyMenu.nextList();
        }
        if (M5.BtnB.wasPressed())
        {
            if (MyMenu.getListString() == "DHT TEMP & HUMIDITY")
            {
                DhtReaderClass DhtReaderObj;
                DhtReaderObj.Run();
            }
            if (MyMenu.getListString() == "WIFI SCANNER")
            {
                WifiScannerClass WifiScannerObj;
                WifiScannerObj.Run();
            }
            if (MyMenu.getListString() == "I2C SCANNER")
            {
               I2CScannerClass I2CScannerObj;
               I2CScannerObj.Run();
            }
            if (MyMenu.getListString() == "WIFI PACKET MONITOR")
            {
               WifiPacketMonitorClass WifiPacketMonitorObj;
               WifiPacketMonitorObj.Run();
            }
            if (MyMenu.getListString() == "STOPWATCH")
            {
                StopWatchClass StopWatchObj;
                StopWatchObj.Run();
            }
        }
        M5.update();
    }
    MyMenu.show();
}
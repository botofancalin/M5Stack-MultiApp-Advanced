#include "DhtReader.h"

void DhtReaderClass::DrawWidgets()
{
    M5.Lcd.drawCentreString("Temperature", 60, 80, 2);
    M5.Lcd.drawCentreString(String(temperature), 60, 100, 4);
    M5.Lcd.drawCentreString("'C", 60, 125, 2);
    M5.Lcd.drawCentreString("Humidity", 260, 80, 2);
    M5.Lcd.drawCentreString(String(humidity), 260, 100, 4);
    M5.Lcd.drawCentreString("%RH", 260, 125, 2);
    M5.Lcd.VprogressBar(120, 50, 20, 100, RED, int(dht.getTemperature()), true);
    M5.Lcd.fillCircle(129, 160, 20, RED);
    M5.Lcd.VprogressBar(180, 50, 20, 100, BLUE, int(dht.getHumidity()), true);
    M5.Lcd.fillCircle(189, 160, 20, BLUE);
}

void DhtReaderClass::Run()
{
    M5.update();
    Wire.begin();
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.drawCentreString("Pin 17 to DHT Signal Pin", 155, 120, 4);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    dht.setup(dhtPin, DHTesp::AUTO_DETECT);
    MyMenu.drawAppMenu(F("DHT Reader"), F("ESC"), F(""), F(""));

    while (!M5.BtnA.wasPressed())
    {
        unsigned long start = millis();
        if (start - past >= 3000)
        {
            temperature = dht.getTemperature();
            humidity = dht.getHumidity();
            if (temperature != oldtemperature || humidity != oldhumidity)
            {
                DrawWidgets();
                oldtemperature = temperature;
                oldhumidity = humidity;
            }
            past = start;
        }
        M5.update();
    }
}

DhtReaderClass::DhtReaderClass()
{
}

DhtReaderClass::~DhtReaderClass()
{
    M5.Lcd.setRotation(0);
    M5.Lcd.fillScreen(0);
    MyMenu.drawAppMenu(F("TOOLS"), F("ESC"), F("SELECT"), F("LIST"));
    MyMenu.showList();
}
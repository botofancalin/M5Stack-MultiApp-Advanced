#include "DhtReader.h"

void DhtReaderClass::DrawWidgets()
{
    M5m.Lcd.drawCentreString("Temperature", 60, 80, 2);
    M5m.Lcd.drawCentreString(String(temperature), 60, 100, 4);
    M5m.Lcd.drawCentreString("'C", 60, 125, 2);
    M5m.Lcd.drawCentreString("Humidity", 260, 80, 2);
    M5m.Lcd.drawCentreString(String(humidity), 260, 100, 4);
    M5m.Lcd.drawCentreString("%RH", 260, 125, 2);
    M5m.Lcd.VprogressBar(120, 50, 20, 100, RED, int(dht.getTemperature()), true);
    M5m.Lcd.fillCircle(129, 160, 20, RED);
    M5m.Lcd.VprogressBar(180, 50, 20, 100, BLUE, int(dht.getHumidity()), true);
    M5m.Lcd.fillCircle(189, 160, 20, BLUE);
}

void DhtReaderClass::Run()
{
    M5m.update();
    M5m.Lcd.fillScreen(BLACK);
    M5m.Lcd.drawCentreString("Pin 17 to DHT Signal Pin", 155, 120, 4);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    dht.setup(dhtPin, DHTesp::AUTO_DETECT);
    M5m.drawAppMenu(F("DHT Reader"), F("ESC"), F(""), F(""));

    while (!M5m.BtnA.wasPressed())
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
        M5m.update();
    }
}

DhtReaderClass::DhtReaderClass()
{
}

DhtReaderClass::~DhtReaderClass()
{
    M5m.Lcd.fillScreen(0);
    M5m.drawAppMenu(F("TOOLS"), F("ESC"), F("SELECT"), F("LIST"));
    M5m.showList();
}
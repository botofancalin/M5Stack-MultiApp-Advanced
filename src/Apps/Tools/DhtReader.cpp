#include "../../apps.h"

unsigned int dhtPin = 17;
unsigned long past = 0;
float temperature, humidity, oldtemperature, oldhumidity;
DHTesp dht;

void Dht_Run()
{
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.drawCentreString("Pin 17 to DHT Signal Pin", 155, 120, 4);
    delay(5000);
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
                M5.Lcd.drawCentreString("Temperature", 60, 80, 2);
                M5.Lcd.drawCentreString(String(temperature), 60, 100, 4);
                M5.Lcd.drawCentreString("'C", 60, 125, 2);
                M5.Lcd.drawCentreString("Humidity", 260, 80, 2);
                M5.Lcd.drawCentreString(String(humidity), 260, 100, 4);
                M5.Lcd.drawCentreString("%RH", 260, 125, 2);
                M5.Lcd.VprogressBar(120, 50, 20, 100, RED, int(dht.getTemperature()));
                M5.Lcd.fillCircle(129, 160, 20, RED);
                M5.Lcd.VprogressBar(180, 50, 20, 100, BLUE, int(dht.getHumidity()));
                M5.Lcd.fillCircle(189, 160, 20, BLUE);
                oldtemperature = temperature;
                oldhumidity = humidity;
            }
            past = start;
        }
        M5.update();
    }
    return;
}
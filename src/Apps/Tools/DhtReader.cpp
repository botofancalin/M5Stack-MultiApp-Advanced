#include "DhtReader.h"

void DhtReaderClass::DrawWidgets()
{
    M5m.Lcd.drawCentreString("Temperature", 60, 80, 2);
    M5m.Lcd.drawCentreString(String(temperature, 2), 60, 100, 4);
    M5m.Lcd.drawCentreString("'C", 60, 125, 2);
    M5m.Lcd.drawCentreString("Humidity", 260, 80, 2);
    M5m.Lcd.drawCentreString(String(humidity, 2), 260, 100, 4);
    M5m.Lcd.drawCentreString("%RH", 260, 125, 2);
    M5m.Lcd.VprogressBar(120, 50, 20, 100, RED, (int)temperature + 20, true);
    M5m.Lcd.fillCircle(129, 160, 20, RED);
    M5m.Lcd.VprogressBar(180, 50, 20, 100, BLUE, (int)humidity - 20, true);
    M5m.Lcd.fillCircle(189, 160, 20, BLUE);
}

void DhtReaderClass::getDHTData()
{
    temperature = NAN;
    humidity = NAN;

    uint16_t rawHumidity = 0;
    uint16_t rawTemperature = 0;
    uint16_t DHTdata = 0;

    // Request sample
    digitalWrite(pin, LOW); // Send start signal
    pinMode(pin, OUTPUT);
    delay(18);
    pinMode(pin, INPUT);
    digitalWrite(pin, HIGH); // Switch bus to receive data

    // We're going to read 83 edges:
    // - First a FALLING, RISING, and FALLING edge for the start bit
    // - Then 40 bits: RISING and then a FALLING edge per bit
    // To keep our code simple, we accept any HIGH or LOW reading if it's max 85 usecs long

    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
    portENTER_CRITICAL(&mux);

    for (int8_t i = -3; i < 2 * 40; i++)
    {
        startTime = micros();

        do
        {
            age = (unsigned long)(micros() - startTime);
            if (age > 90)
            {
                portEXIT_CRITICAL(&mux);
                return;
            }
        } while (digitalRead(pin) == (i & 1) ? HIGH : LOW);

        if (i >= 0 && (i & 1))
        {
            DHTdata <<= 1;
            if (age > 30)
            {
                DHTdata |= 1; // we got a one
            }
        }

        switch (i)
        {
        case 31:
            rawHumidity = DHTdata;
            break;
        case 63:
            rawTemperature = DHTdata;
            DHTdata = 0;
            break;
        }
    }
    portEXIT_CRITICAL(&mux);
    // Verify checksum
    if ((byte)(((byte)rawHumidity) + (rawHumidity >> 8) + ((byte)rawTemperature) + (rawTemperature >> 8)) != DHTdata)
    {
        return;
    }
    humidity = rawHumidity >> 8;
    temperature = rawTemperature >> 8;
}

void DhtReaderClass::getSHTData(uint8_t _address)
{
    Wire.beginTransmission(_address);
    Wire.write(0x2C);
    Wire.write(0x06);
    Wire.endTransmission();
    delay(500);
    Wire.requestFrom((int)_address, 6);

    for (int i = 0; i < 6; i++)
    {
        data[i] = Wire.read();
    }

    temperature = ((((data[0] * 256.0) + data[1]) * 175) / 65535.0) - t_dr;
    humidity = ((((data[3] * 256.0) + data[4]) * 100) / 65535.0) + h_dr;
}

void DhtReaderClass::Run()
{
    M5m.update();
    M5m.Lcd.fillScreen(BLACK);
    M5m.drawAppMenu(F("SHT Sensor"), F("ESC"), F("SHT"), F(""));

    while (!M5m.BtnA.wasPressed())
    {
        unsigned long start = millis();
        if (start - past >= 3000)
        {
            if (SHT)
            {
                getSHTData(_address);
                if (sw)
                {
                    M5m.drawAppMenu(F("SHT Sensor"), F("ESC"), F("DHT"), F(""));
                    sw = false;
                }
            }

            else
            {
                getDHTData();
                if (sw)
                {
                    M5m.drawAppMenu(F("DHT Sensor Drift Comp"), F("ESC"), F("SHT"), F(""));
                    sw = false;
                }
            }

            if (temperature != oldtemperature || humidity != oldhumidity)
            {
                DrawWidgets();
                oldtemperature = temperature;
                oldhumidity = humidity;
            }
            past = start;
        }

        if (M5m.BtnB.wasPressed())
        {
            SHT = !SHT;
            sw = true;
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
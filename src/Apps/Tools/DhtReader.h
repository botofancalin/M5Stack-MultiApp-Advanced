#pragma once
#include "M5Stack.h"

class DhtReaderClass
{
  public:
    DhtReaderClass();
    ~DhtReaderClass();

    void Run();

  private:
    unsigned int dhtPin = 17;
    unsigned long past = 0;
    float temperature, humidity, oldtemperature, oldhumidity;
    DHTesp dht;

    void DrawWidgets();
};

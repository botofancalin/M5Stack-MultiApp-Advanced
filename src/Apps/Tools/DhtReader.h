#pragma once
#include "M5StackMod.h"

class DhtReaderClass
{
public:
  DhtReaderClass();
  ~DhtReaderClass();

  void Run();

private:
  unsigned long past = 0, sht_wait = 0, startTime = 0, start, sht_start;
  float temperature = 0, humidity = 0, oldtemperature, oldhumidity;
  unsigned int data[6];
  uint8_t _address = 0x45, pin = 17, age;
  bool SHT = true, sw = true;

  void DrawWidgets();
  void getSHTData(uint8_t _address);
  void getDHTData();
};

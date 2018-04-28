// Copyright (c) M5Stack. All rights reserved.

// Licensed under the MIT license. See LICENSE file in the project root for full license information.
/**
 * \par Copyright (C), 2016-2017, M5Stack
 * \class M5Stack
 * \brief   M5Stack library.
 * @file    M5Stack.h
 * @author  M5Stack
 * @version V0.1.5
 * @date    2017/03/27
 * @brief   Header for M5Stack.cpp module
 *
 * \par Description
 * This file is a drive for M5Stack core.
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Zibin Zheng         2017/07/14        0.0.1          Rebuild the new.
 * </pre>
 *
 */

#ifndef _M5STACK_H_
#define _M5STACK_H_

#if defined(ESP32)

// #define MPU9250_INSDE


#include <Arduino.h>
#include "WiFi.h"
#include "WiFiClient.h"
#include "WiFiMulti.h"
#include <Wire.h>
#include <SPI.h>
#include "FS.h"
#include "SD.h"
#include "SPIFFS.h"
#include "Preferences.h"

#include "utility/Display.h"
#include "utility/Config.h"
#include "utility/Button.h"

extern "C"
{
#include "esp_sleep.h"
}

class M5Stack {

 public:
     M5Stack();
    ~M5Stack();
    void begin();
    void update();

    void setWakeupButton(uint8_t button);
    void powerOFF();

    // Button API
    #define DEBOUNCE_MS 5
    Button BtnA = Button(BUTTON_A_PIN, true, DEBOUNCE_MS);
    Button BtnB = Button(BUTTON_B_PIN, true, DEBOUNCE_MS);
    Button BtnC = Button(BUTTON_C_PIN, true, DEBOUNCE_MS);

    // LCD
    ILI9341 Lcd = ILI9341();

    // UART
    HardwareSerial Serial0 = HardwareSerial(0);
    HardwareSerial Serial2 = HardwareSerial(2);

 private:
    uint8_t _wakeupPin;
};

extern M5Stack M5;
#define m5 M5
#define lcd Lcd

#else
#error “This library only supports boards with ESP32 processor.”
#endif

#endif

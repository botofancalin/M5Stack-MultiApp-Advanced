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

#include <Arduino.h>
#include <vector>
#include "WiFi.h"
#include "WiFiClient.h"
#include <ESPmDNS.h>
#include <Wire.h>
#include <SPI.h>
#include "FS.h"
#include "SD.h"
#include "SPIFFS.h"
#include "DHTesp.h"
#include "Preferences.h"
#include "ArduinoOTA.h"
#include "WebServer.h"
#include <Update.h>
#include <HTTPClient.h>

#include "utility/Display.h"
#include "utility/Config.h"
#include "utility/Button.h"
#include "AudioFileSourceSD.h"
#include "AudioFileSourceICYStream.h"
#include "AudioFileSourceBuffer.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

#define MENU_TITLE_MAX_SIZE 24
#define BTN_TITLE_MAX_SIZE 6
#define MAX_SUBMENUS 8

#define LIST_MAX_LABEL_SIZE 64
#define LIST_PAGE_LABELS 6

static int WiFi_Mode;
static int vol, old_vol;

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
      void up();
  void down();
  void execute();
  void windowClr();
  void setColorSchema(unsigned int inmenucolor, unsigned int inwindowcolor, unsigned int intextcolor);
  void drawAppMenu(String inmenuttl, String inbtnAttl, String inbtnBttl, String inbtnCttl);
  void GoToLevel(uint32_t inlevel);
  unsigned int getrgb(uint8_t inred, uint8_t ingrn, uint8_t inblue);
  void addMenuItem(uint32_t levelID, const char *menu_title, const char *btnA_title, const char *btnB_title, const char *btnC_title, 
  signed char goto_level, const char *Menu_Img, void (*function)());
  void show();
  void showList();
  void clearList();
  unsigned int getListID();
  String getListString();
  void nextList();
  void addList(String inLabel);
  void setListCaption(String inCaption);
  void btnRestore();
  String lastBtnTittle[3];

    void setWakeupButton(uint8_t button);
    void powerOFF();

    // Button API
    #define DEBOUNCE_MS 5
    Button BtnA = Button(BUTTON_A_PIN, true, DEBOUNCE_MS);
    Button BtnB = Button(BUTTON_B_PIN, true, DEBOUNCE_MS);
    Button BtnC = Button(BUTTON_C_PIN, true, DEBOUNCE_MS);

    // LCD
    ILI9341 Lcd = ILI9341();

 private:
    uint8_t _wakeupPin;
    String listCaption;
  void drawListItem(uint32_t inIDX, uint32_t postIDX);
  void drawMenu(String inmenuttl, String inbtnAttl, String inbtnBttl, String inbtnCttl, unsigned int inmenucolor, 
  unsigned int inwindowcolor, const char *iMenuImg, unsigned int intxtcolor);
  struct MenuCommandCallback
  {
    char title[MENU_TITLE_MAX_SIZE + 1];
    char btnAtitle[BTN_TITLE_MAX_SIZE + 1];
    char btnBtitle[BTN_TITLE_MAX_SIZE + 1];
    char btnCtitle[BTN_TITLE_MAX_SIZE + 1];
    signed char gotoLevel;
    const char *MenuImg;
    void (*function)();
  };
  std::vector<String> list_labels;
  uint32_t list_lastpagelines;
  uint32_t list_count;
  uint32_t list_pages;
  uint32_t list_page;
  unsigned int list_idx;
  uint32_t list_lines;

  MenuCommandCallback *menuList[MAX_SUBMENUS];
  uint32_t menuIDX;
  uint32_t levelIDX;
  uint32_t menuCount[MAX_SUBMENUS];
  unsigned int menucolor;
  unsigned int windowcolor;
  unsigned int menutextcolor;
};

extern Preferences preferences;
extern M5Stack M5;
#define m5 M5
#define lcd Lcd

#else
#error “This library only supports boards with ESP32 processor.”
#endif

#endif

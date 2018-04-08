#include "M5Stack.h"

#if defined(WIRING) && WIRING >= 100
#include <Wiring.h>
#elif defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#ifndef M5StackSAM_h
#define M5StackSAM_h

#define M5SAM_MENU_TITLE_MAX_SIZE 24
#define M5SAM_BTN_TITLE_MAX_SIZE 6
#define M5SAM_MAX_SUBMENUS 8

#define M5SAM_LIST_MAX_COUNT 32
#define M5SAM_LIST_MAX_LABEL_SIZE 36
#define M5SAM_LIST_PAGE_LABELS 6

volatile static uint8_t _keyboardIRQRcvd;
volatile static uint8_t _keyboardChar;

class M5SAM
{
public:
  M5SAM();
  void up();
  void down();
  void execute();
  void windowClr();
  void setColorSchema(unsigned int inmenucolor, unsigned int inwindowcolor, unsigned int intextcolor);
  void drawAppMenu(String inmenuttl, String inbtnAttl, String inbtnBttl, String inbtnCttl);
  void GoToLevel(byte inlevel);
  unsigned int getrgb(byte inred, byte ingrn, byte inblue);
  void addMenuItem(byte levelID, const char *menu_title, const char *btnA_title, const char *btnB_title, const char *btnC_title, signed char goto_level, const char* Menu_Img, void (*function)());
  void show();
  void showList();
  void clearList();
  byte getListID();
  String getListString();
  void nextList();
  void addList(String inLabel);
  void setListCaption(String inCaption);
  String keyboardGetString();
  String lastBtnTittle[3];

private:
  String listCaption;
  void drawListItem(byte inIDX, byte postIDX);

  void btnRestore();
  void keyboardEnable();
  void keyboardDisable();
  static void keyboardIRQ();

  void drawMenu(String inmenuttl, String inbtnAttl, String inbtnBttl, String inbtnCttl, unsigned int inmenucolor, unsigned int inwindowcolor, const char *iMenuImg, unsigned int intxtcolor);
  struct MenuCommandCallback
  {
    char title[M5SAM_MENU_TITLE_MAX_SIZE + 1];
    char btnAtitle[M5SAM_BTN_TITLE_MAX_SIZE + 1];
    char btnBtitle[M5SAM_BTN_TITLE_MAX_SIZE + 1];
    char btnCtitle[M5SAM_BTN_TITLE_MAX_SIZE + 1];
    signed char gotoLevel;
    const char *MenuImg;
    void (*function)();
  };
  String list_labels[M5SAM_LIST_MAX_COUNT];
  byte list_lastpagelines;
  byte list_count;
  byte list_pages;
  byte list_page;
  byte list_idx;
  byte list_lines;

  MenuCommandCallback *menuList[M5SAM_MAX_SUBMENUS];
  byte menuIDX;
  byte levelIDX;
  byte menuCount[M5SAM_MAX_SUBMENUS];
  unsigned int menucolor;
  unsigned int windowcolor;
  unsigned int menutextcolor;
};

extern M5SAM MyMenu;

#endif

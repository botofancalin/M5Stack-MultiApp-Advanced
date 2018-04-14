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
  void GoToLevel(uint8_t inlevel);
  unsigned int getrgb(uint8_t inred, uint8_t ingrn, uint8_t inblue);
  void addMenuItem(uint8_t levelID, const char *menu_title, const char *btnA_title, const char *btnB_title, const char *btnC_title, signed char goto_level, const char* Menu_Img, void (*function)());
  void show();
  void showList();
  void clearList();
  uint8_t getListID();
  String getListString();
  void nextList();
  void addList(String inLabel);
  void setListCaption(String inCaption);
  void btnRestore();
  String lastBtnTittle[3];

private:
  String listCaption;
  void drawListItem(uint8_t inIDX, uint8_t postIDX);
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
  uint8_t list_lastpagelines;
  uint8_t list_count;
  uint8_t list_pages;
  uint8_t list_page;
  uint8_t list_idx;
  uint8_t list_lines;

  MenuCommandCallback *menuList[M5SAM_MAX_SUBMENUS];
  uint8_t menuIDX;
  uint8_t levelIDX;
  uint8_t menuCount[M5SAM_MAX_SUBMENUS];
  unsigned int menucolor;
  unsigned int windowcolor;
  unsigned int menutextcolor;
};

extern M5SAM MyMenu;

#endif

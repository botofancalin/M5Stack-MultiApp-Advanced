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

#define M5SAM_LIST_MAX_LABEL_SIZE 64
#define M5SAM_LIST_PAGE_LABELS 6

static int WiFi_Mode;
static int vol, old_vol;

class M5SAM
{
public:
  M5SAM();
  ~M5SAM();
  void up();
  void down();
  void execute();
  void windowClr();
  void setColorSchema(unsigned int inmenucolor, unsigned int inwindowcolor, unsigned int intextcolor);
  void drawAppMenu(String inmenuttl, String inbtnAttl, String inbtnBttl, String inbtnCttl);
  void GoToLevel(uint32_t inlevel);
  unsigned int getrgb(uint32_t inred, uint32_t ingrn, uint32_t inblue);
  void addMenuItem(uint32_t levelID, const char *menu_title, const char *btnA_title, const char *btnB_title, const char *btnC_title, signed char goto_level, const uint8_t *Menu_Img, void (*function)());
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

private:
  String listCaption;
  void drawListItem(uint32_t inIDX, uint32_t postIDX);
  void drawMenu(String inmenuttl, String inbtnAttl, String inbtnBttl, String inbtnCttl, unsigned int inmenucolor, unsigned int inwindowcolor, const uint8_t *iMenuImg, unsigned int intxtcolor);
  struct MenuCommandCallback
  {
    char title[M5SAM_MENU_TITLE_MAX_SIZE + 1];
    char btnAtitle[M5SAM_BTN_TITLE_MAX_SIZE + 1];
    char btnBtitle[M5SAM_BTN_TITLE_MAX_SIZE + 1];
    char btnCtitle[M5SAM_BTN_TITLE_MAX_SIZE + 1];
    signed char gotoLevel;
    const uint8_t *MenuImg;
    void (*function)();
  };
  std::vector<String> list_labels;
  uint32_t list_lastpagelines;
  uint32_t list_count;
  uint32_t list_pages;
  uint32_t list_page;
  unsigned int list_idx;
  uint32_t list_lines;

  MenuCommandCallback *menuList[M5SAM_MAX_SUBMENUS];
  uint32_t menuIDX;
  uint32_t levelIDX;
  uint32_t menuCount[M5SAM_MAX_SUBMENUS];
  unsigned int menucolor;
  unsigned int windowcolor;
  unsigned int menutextcolor;
};

extern M5SAM MyMenu;

#endif

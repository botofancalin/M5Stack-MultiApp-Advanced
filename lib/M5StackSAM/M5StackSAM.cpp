#include "M5StackSAM.h"

M5SAM::M5SAM()
{
  levelIDX = 0;
  menuCount[levelIDX] = 0;
  menuIDX = 0;
  menucolor = getrgb(0, 0, 128);
  windowcolor = getrgb(0, 0, 0);
  menutextcolor = getrgb(255, 255, 255);
  clearList();
}

void M5SAM::setListCaption(String inCaption)
{
  listCaption = inCaption;
}

void M5SAM::clearList()
{
  list_count = 0;
  list_pages = 0;
  list_page = 0;
  list_lastpagelines = 0;
  list_idx = 0;
  list_labels.clear();
  list_labels.shrink_to_fit();
  listCaption = "";
}

void M5SAM::addList(String inStr)
{
  if (inStr.length() <= M5SAM_LIST_MAX_LABEL_SIZE && inStr.length() > 0)
  {
    list_labels.push_back(inStr);
    list_count++;
  }
  if (list_count > 0)
  {
    if (list_count > M5SAM_LIST_PAGE_LABELS)
    {
      list_lastpagelines = list_count % M5SAM_LIST_PAGE_LABELS;
      if (list_lastpagelines > 0)
      {
        list_pages = (list_count - list_lastpagelines) / M5SAM_LIST_PAGE_LABELS;
        list_pages++;
      }
      else
      {
        list_pages = list_count / M5SAM_LIST_PAGE_LABELS;
      }
    }
    else
    {
      list_pages = 1;
    }
  }
}

unsigned int M5SAM::getListID()
{
  return list_idx;
}

String M5SAM::getListString()
{
  return list_labels[list_idx];
}

void M5SAM::nextList()
{
  if (list_idx < list_page * M5SAM_LIST_PAGE_LABELS + list_lines - 1)
  {
    list_idx++;
  }
  else
  {
    if (list_page < list_pages - 1)
    {
      list_page++;
    }
    else
    {
      list_page = 0;
    }
    list_idx = list_page * M5SAM_LIST_PAGE_LABELS;
  }
  showList();
}

void M5SAM::drawListItem(uint32_t inIDX, uint32_t postIDX)
{
  if (inIDX == list_idx)
  {
    M5.Lcd.drawString(list_labels[inIDX], 15, 80 + (postIDX * 20), 2);
    M5.Lcd.drawString(">", 3, 80 + (postIDX * 20), 2);
  }
  else
  {
    M5.Lcd.drawString(list_labels[inIDX], 15, 80 + (postIDX * 20), 2);
  }
}

void M5SAM::showList()
{
  windowClr();
  unsigned int labelid = 0;
  M5.Lcd.drawCentreString(listCaption, M5.Lcd.width() / 2, 45, 2);
  if ((list_page + 1) == list_pages)
  {
    if (list_lastpagelines == 0 && list_count >= M5SAM_LIST_PAGE_LABELS)
    {
      list_lines = M5SAM_LIST_PAGE_LABELS;
      for (uint32_t i = 0; i < M5SAM_LIST_PAGE_LABELS; i++)
      {
        labelid = i + (list_page * M5SAM_LIST_PAGE_LABELS);
        drawListItem(labelid, i);
      }
    }
    else
    {
      if (list_pages > 1)
      {
        list_lines = list_lastpagelines;
        for (uint32_t i = 0; i < list_lastpagelines; i++)
        {
          labelid = i + (list_page * M5SAM_LIST_PAGE_LABELS);
          drawListItem(labelid, i);
        }
      }
      else
      {
        list_lines = list_count;
        for (uint32_t i = 0; i < list_count; i++)
        {
          labelid = i + (list_page * M5SAM_LIST_PAGE_LABELS);
          drawListItem(labelid, i);
        }
      }
    }
  }
  else
  {
    list_lines = M5SAM_LIST_PAGE_LABELS;
    for (uint32_t i = 0; i < M5SAM_LIST_PAGE_LABELS; i++)
    {
      labelid = i + (list_page * M5SAM_LIST_PAGE_LABELS);
      drawListItem(labelid, i);
    }
  }
}

void M5SAM::up()
{
  if (menuIDX < menuCount[levelIDX] - 1)
  {
    menuIDX++;
    show();
  }
}

void M5SAM::down()
{
  if (menuIDX > 0)
  {
    menuIDX--;
    show();
  }
}

void M5SAM::GoToLevel(uint32_t inlevel)
{
  levelIDX = inlevel;
  menuIDX = 0;
  show();
}

void M5SAM::execute()
{
  if (menuList[levelIDX][menuIDX].gotoLevel == -1)
  {
    (*menuList[levelIDX][menuIDX].function)();
  }
  else
  {
    GoToLevel(menuList[levelIDX][menuIDX].gotoLevel);
  }
}

void M5SAM::addMenuItem(uint32_t levelID, const char *menu_title, const char *btnA_title, const char *btnB_title, 
const char *btnC_title, signed char goto_level, const uint8_t *Menu_Img, void (*function)())
{
  uint32_t mCnt = menuCount[levelID];
  menuList[levelID] = (MenuCommandCallback *)realloc(menuList[levelID], (mCnt + 1) * sizeof(MenuCommandCallback));
  strncpy(menuList[levelID][mCnt].title, menu_title, M5SAM_MENU_TITLE_MAX_SIZE);
  strncpy(menuList[levelID][mCnt].btnAtitle, btnA_title, M5SAM_BTN_TITLE_MAX_SIZE);
  strncpy(menuList[levelID][mCnt].btnBtitle, btnB_title, M5SAM_BTN_TITLE_MAX_SIZE);
  strncpy(menuList[levelID][mCnt].btnCtitle, btnC_title, M5SAM_BTN_TITLE_MAX_SIZE);
  menuList[levelID][mCnt].gotoLevel = goto_level;
  menuList[levelID][mCnt].MenuImg = Menu_Img;
  menuList[levelID][mCnt].function = function;
  menuCount[levelID]++;
}

void M5SAM::show()
{
  drawMenu(menuList[levelIDX][menuIDX].title, menuList[levelIDX][menuIDX].btnAtitle, menuList[levelIDX][menuIDX].btnBtitle,
           menuList[levelIDX][menuIDX].btnCtitle, menucolor, windowcolor, menuList[levelIDX][menuIDX].MenuImg, menutextcolor);
}

void M5SAM::windowClr()
{
  M5.Lcd.fillRoundRect(0, 29, M5.Lcd.width(), M5.Lcd.height() - 28 - 28, 3, windowcolor);
}

unsigned int M5SAM::getrgb(uint32_t inred, uint32_t ingrn, uint32_t inblue)
{
  inred = map(inred, 0, 255, 0, 31);
  ingrn = map(ingrn, 0, 255, 0, 63);
  inblue = map(inblue, 0, 255, 0, 31);
  return inred << 11 | ingrn << 5 | inblue;
}

void M5SAM::drawAppMenu(String inmenuttl, String inbtnAttl, String inbtnBttl, String inbtnCttl)
{
  drawMenu(inmenuttl, inbtnAttl, inbtnBttl, inbtnCttl, menucolor, windowcolor, NULL, menutextcolor);
  M5.Lcd.setTextColor(menutextcolor, windowcolor);
}

void M5SAM::setColorSchema(unsigned int inmenucolor, unsigned int inwindowcolor, unsigned int intextcolor)
{
  menucolor = inmenucolor;
  windowcolor = inwindowcolor;
  menutextcolor = intextcolor;
}

void M5SAM::btnRestore()
{
  M5.Lcd.setTextColor(menutextcolor);
  M5.Lcd.fillRoundRect(0, M5.Lcd.height() - 28, M5.Lcd.width(), 28, 3, 0x00);
  M5.Lcd.fillRoundRect(31, M5.Lcd.height() - 28, 60, 28, 3, menucolor);
  M5.Lcd.fillRoundRect(126, M5.Lcd.height() - 28, 60, 28, 3, menucolor);
  M5.Lcd.fillRoundRect(221, M5.Lcd.height() - 28, 60, 28, 3, menucolor);
  M5.Lcd.drawCentreString(lastBtnTittle[0], 31 + 30, M5.Lcd.height() - 28 + 6, 2);
  M5.Lcd.drawCentreString(lastBtnTittle[1], 126 + 30, M5.Lcd.height() - 28 + 6, 2);
  M5.Lcd.drawCentreString(lastBtnTittle[2], 221 + 30, M5.Lcd.height() - 28 + 6, 2);
  M5.Lcd.setTextColor(menutextcolor, windowcolor);
}

void M5SAM::drawMenu(String inmenuttl, String inbtnAttl, String inbtnBttl, String inbtnCttl, unsigned int inmenucolor, 
unsigned int inwindowcolor, const uint8_t *iMenuImg, unsigned int intxtcolor)
{
  lastBtnTittle[0] = inbtnAttl;
  lastBtnTittle[1] = inbtnBttl;
  lastBtnTittle[2] = inbtnCttl;
  M5.Lcd.fillRoundRect(31, M5.Lcd.height() - 28, 60, 28, 3, inmenucolor);
  M5.Lcd.fillRoundRect(126, M5.Lcd.height() - 28, 60, 28, 3, inmenucolor);
  M5.Lcd.fillRoundRect(221, M5.Lcd.height() - 28, 60, 28, 3, inmenucolor);
  M5.Lcd.fillRoundRect(0, 0, M5.Lcd.width(), 28, 3, inmenucolor);
  M5.Lcd.fillRoundRect(0, 29, M5.Lcd.width(), M5.Lcd.height() - 28 - 28, 3, inwindowcolor);
  if (iMenuImg != NULL)
  {
    M5.Lcd.drawJpg(iMenuImg, (sizeof(iMenuImg)/sizeof(iMenuImg[0])), 0, 30);
  }

  M5.Lcd.setTextColor(intxtcolor);
  M5.Lcd.drawCentreString(inmenuttl, M5.Lcd.width() / 2, 6, 2);

  M5.Lcd.drawCentreString(inbtnAttl, 31 + 30, M5.Lcd.height() - 28 + 6, 2);
  M5.Lcd.drawCentreString(inbtnBttl, 126 + 30, M5.Lcd.height() - 28 + 6, 2);
  M5.Lcd.drawCentreString(inbtnCttl, 221 + 30, M5.Lcd.height() - 28 + 6, 2);
}

M5SAM::~M5SAM()
{
}

M5SAM MyMenu;
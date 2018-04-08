#include "../apps.h"


void appListGames()
{
    MyMenu.drawAppMenu(F("GAMES"),F("ESC"),F("SELECT"),F("LIST"));

    while (M5.BtnB.wasPressed())
    {
        M5.update();
    }

    MyMenu.clearList();
    MyMenu.setListCaption("GAMES");
    MyMenu.addList("ALIEN SHOOTER");
    MyMenu.addList("FLAPPY BIRD");
    MyMenu.showList();

    while (!M5.BtnA.wasPressed())
    {
        if (M5.BtnC.wasPressed())
        {
            MyMenu.nextList();
        }
        if (M5.BtnB.wasPressed())
        {
            if (MyMenu.getListString() == "ALIEN SHOOTER")
            {
                M5.update();
                spaceShootest_run();
                M5.Lcd.fillScreen(0);
				M5.Lcd.setTextSize(1);
                MyMenu.drawAppMenu(F("GAMES"),F("ESC"),F("SELECT"),F("LIST"));
                MyMenu.showList();
            }
             if (MyMenu.getListString() == "FLAPPY BIRD")
            {
                M5.update();
                flappypird_run();
                M5.Lcd.fillScreen(0);
				M5.Lcd.setTextSize(1);
                MyMenu.drawAppMenu(F("GAMES"),F("ESC"),F("SELECT"),F("LIST"));
                MyMenu.showList();
            }
        }
        M5.update();
    }
    MyMenu.show();
}
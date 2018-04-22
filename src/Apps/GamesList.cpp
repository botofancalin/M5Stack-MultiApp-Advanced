#include "../Commons.h"

// the game list menu
void appListGames()
{
    MyMenu.drawAppMenu(F("GAMES"),F("ESC"),F("SELECT"),F("LIST"));

    while (M5.BtnB.wasPressed())
    {
        M5.update();
    }

    MyMenu.clearList();
    MyMenu.setListCaption("GAMES");

    // The list items Add new items to list to be displayed
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
            //The items from list must be found into this funtions.
            if (MyMenu.getListString() == "ALIEN SHOOTER")
            {
                M5.update();
                // The game function
                spaceShootest_run();
                M5.Lcd.fillScreen(0);
				M5.Lcd.setTextSize(1);
                MyMenu.drawAppMenu(F("GAMES"),F("ESC"),F("SELECT"),F("LIST"));
                MyMenu.showList();
            }
             if (MyMenu.getListString() == "FLAPPY BIRD")
            {
                M5.update();
                // The game function
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
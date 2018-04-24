#include "GamesList.h"

// the game list menu
void GamesListClass::Run()
{
    M5.update();
    MyMenu.drawAppMenu(F("GAMES"),F("ESC"),F("SELECT"),F("LIST"));
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
                // The game object with member call
                SpaceShooterClass SpaceShooterObj;
                SpaceShooterObj.Run();
            }
             if (MyMenu.getListString() == "FLAPPY BIRD")
            {
                M5.update();
                FlappyBirdClass FlappyBirdObj;
                FlappyBirdObj.Run();
            }
        }
        M5.update();
    } 
}

GamesListClass::GamesListClass()
{
}

GamesListClass::~GamesListClass()
{
    MyMenu.show();
}
#include "GamesList.h"

// the game list menu
void GamesListClass::Run()
{
    M5.update();
    M5.drawAppMenu(F("GAMES"),F("ESC"),F("SELECT"),F("LIST"));
    M5.clearList();
    M5.setListCaption("GAMES");

    // The list items Add new items to list to be displayed
    M5.addList("ALIEN SHOOTER");
    M5.addList("FLAPPY BIRD");
    M5.showList();

    while (!M5.BtnA.wasPressed())
    {
        if (M5.BtnC.wasPressed())
        {
            M5.nextList();
        }
        if (M5.BtnB.wasPressed())
        {
            if (M5.getListString() == "ALIEN SHOOTER")
            {
                M5.update();
                // The game object with member call
                SpaceShooterClass SpaceShooterObj;
                SpaceShooterObj.Run();
            }
             if (M5.getListString() == "FLAPPY BIRD")
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
    M5.show();
}
#include "GamesList.h"

// the game list menu
void GamesListClass::Run()
{
    M5m.update();
    M5m.drawAppMenu(F("GAMES"),F("ESC"),F("SELECT"),F("LIST"));
    M5m.clearList();
    M5m.setListCaption("GAMES");

    // The list items Add new items to list to be displayed
        M5m.addList("TETRIS");
            M5m.addList("FLAPPY BIRD");
    M5m.addList("ALIEN SHOOTER");
    M5m.showList();

    while (!M5m.BtnA.wasPressed())
    {
        if (M5m.BtnC.wasPressed())
        {
            M5m.nextList();
        }
        if (M5m.BtnB.wasPressed())
        {
            if (M5m.getListString() == "TETRIS")
            {
                M5m.update();
                TetrisClass* TetrisObj = new TetrisClass();
                TetrisObj->Run();
                TetrisClass::DestroyInstance(TetrisObj);
            }
                        if (M5m.getListString() == "FLAPPY BIRD")
            {
                M5m.update();
                FlappyBirdClass FlappyBirdObj;
                FlappyBirdObj.Run();
            }
            if (M5m.getListString() == "ALIEN SHOOTER")
            {
                M5m.update();
                // The game object with member call
                SpaceShooterClass SpaceShooterObj;
                SpaceShooterObj.Run();
            }
        }
        M5m.update();
    } 
}

GamesListClass::GamesListClass()
{
}

GamesListClass::~GamesListClass()
{
    M5m.show();
}
#pragma once
#include "M5StackMod.h"

#define TFTW 320  // screen width
#define TFTH 240  // screen height
#define TFTW2 160 // half screen width
#define TFTH2 120 // half screen height
// game constant
#define SPEED 1
#define GRAVITY 9.8
#define JUMP_FORCE 2.15
#define SKIP_TICKS 20.0 // 1000 / 50fps
#define MAX_FRAMESKIP 5
// bird size
#define BIRDW 16 // bird width
#define BIRDH 16 // bird height
#define BIRDW2 8 // half width
#define BIRDH2 8 // half height
// pipe size
#define PIPEW 24     // pipe width
#define GAPHEIGHT 56 // pipe gap height
// floor size
#define FLOORH 30 // floor height (from bottom of the screen)
// grass size
#define GRASSH 4 // grass height (inside floor, starts at floor y)

// bird sprite
// bird sprite colors (Cx name for values to keep the array readable)
#define C0 BCKGRDCOL
#define C1 M5m.Lcd.color565(195, 165, 75)
#define C2 BIRDCOL
#define C3 WHITE
#define C4 RED
#define C5 M5m.Lcd.color565(251, 216, 114)

class FlappyBirdClass
{
  public:
    FlappyBirdClass();
    ~FlappyBirdClass();

    void Run();

  private:
    int maxflappy_bird_score = 0;
    // background
    const unsigned int BCKGRDCOL = M5m.Lcd.color565(138, 235, 244);
    // bird
    const unsigned int BIRDCOL = M5m.Lcd.color565(255, 254, 174);
    // pipe
    const unsigned int PIPECOL = M5m.Lcd.color565(99, 255, 78);
    // pipe highlight
    const unsigned int PIPEHIGHCOL = M5m.Lcd.color565(250, 255, 250);
    // pipe seam
    const unsigned int PIPESEAMCOL = M5m.Lcd.color565(0, 0, 0);
    // floor
    const unsigned int FLOORCOL = M5m.Lcd.color565(246, 240, 163);
    // grass (col2 is the stripe color)
    const unsigned int GRASSCOL = M5m.Lcd.color565(141, 225, 87);
    const unsigned int GRASSCOL2 = M5m.Lcd.color565(156, 239, 88);

    unsigned int birdcol[128] =
        {C0, C0, C1, C1, C1, C1, C1, C0, C0, C0, C1, C1, C1, C1, C1, C0,
         C0, C1, C2, C2, C2, C1, C3, C1, C0, C1, C2, C2, C2, C1, C3, C1,
         C0, C2, C2, C2, C2, C1, C3, C1, C0, C2, C2, C2, C2, C1, C3, C1,
         C1, C1, C1, C2, C2, C3, C1, C1, C1, C1, C1, C2, C2, C3, C1, C1,
         C1, C2, C2, C2, C2, C2, C4, C4, C1, C2, C2, C2, C2, C2, C4, C4,
         C1, C2, C2, C2, C1, C5, C4, C0, C1, C2, C2, C2, C1, C5, C4, C0,
         C0, C1, C2, C1, C5, C5, C5, C0, C0, C1, C2, C1, C5, C5, C5, C0,
         C0, C0, C1, C5, C5, C5, C0, C0, C0, C0, C1, C5, C5, C5, C0, C0};

    // bird structure
    struct BIRD
    {
        long x, y, old_y;
        long col;
        float vel_y;
    } bird;

    // pipe structure
    struct PIPES
    {
        long x, gap_y;
        long col;
    } pipes;

    // flappy_bird_score
    int flappy_bird_score;
    // temporary x and y var
    short tmpx, tmpy;

    void game_over();
    void resetMaxflappy_bird_score();
    void game_start();
    void game_init();
    void game_loop();
};
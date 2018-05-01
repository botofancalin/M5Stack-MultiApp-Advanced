#pragma once
#include "M5Stack.h"

class SpaceShooterClass
{
  public:
    SpaceShooterClass();
    ~SpaceShooterClass();

    void Run();

  private:
    //============================= game variables =========================
    unsigned long offsetM;
    unsigned long offsetT;
    unsigned long offsetF;
    unsigned long offsetB;
    unsigned long offsetA;
    unsigned long offsetAF;
    unsigned long offsetAB;
    unsigned long offsetS;
    int threshold;
    bool startPrinted;
    bool beginGame;
    bool beginGame2;
    bool play;
    int alien_shooter_score;
    int alien_shooter_scoreInc;
    int level;
    //---------------------Player---------------------------------------
    int shipX;
    int shipY;
    int oldShipX;
    int oldShipY;
    int changeShipX;
    int changeShipY;
    int shipSpeed;
    bool doSplode;
    bool fire;
    int fFireX[5];
    int fFireY[5];
    int fFireAge[5];
    //--------------------------Aliens----------------------------------
    bool alienLive[18];
    int alienLiveCount;
    int alienX;
    int alienY;
    int oldAlienX;
    int oldAlienY;
    int changeAlienX;
    int changeAlienY;
    int alienSpeed;
    int oldAlienSpeed;
    int aFireX[5];
    int aFireY[5];
    bool aFireAge[5];
    int chanceOfFire;
    //================================ bitmaps ========================
    //your starship
    const int shipImgW = 14;
    const int shipImgH = 16;
    char shipImg[225] = "ZZZZZZWWZZZZZZZZZZZYWWYZZZZZZZZZZWWWWZZZZZZZZZZ"
                        "WWWWZZZZZZZZZWWWWWWZZZZZZZZWWWWWWZZZZZYZZWWWWWWZZYZZYZZWWWWWWZZ"
                        "YZWWZZWWBBWWZZWWWWZZWBBBBWZZWWWWZWWBBBBWWZWWWWZWWWWWWWWZWWWWWWW"
                        "WWWWWWWWWWRWWWWWWWWWWRWZZWWWWWWWWWWZZZZZWRRWWRRWZZZ";
    //flames
    const int flamesImgW = 12;
    const int flamesImgH = 6;
    char flamesImg[73] = "RZZZZZZZZZZRRZRYYRRYYRZRRZRYYRRYYRZRZZRYRZZRY"
                         "RZZZZZRZZZZRZZZZZZRZZZZRZZZ";
    //alien
    const int alienImgW = 14;
    const int alienImgH = 11;
    char alienImg[155] = "GGGZZZZZZZZGGGZZZGZZZZZZGZZZZZGGGGGGGGGGZZZGGG"
                         "GGGGGGGGGZGGGZGGGGGGZGGGGGGZZGGGGZZGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
                         "GGGGZZZGGZZGGZZZGZGGZGGZZGGZGGZZZZZZGZZGZZZZZ";
    //ship 'sploded
    const int splodedImgW = 14;
    const int splodedImgH = 16;
    char splodedImg[225] = "ZZZZZZWWZZZZZZZZZZRYWWYRZZZYZZZRRWWRRRRZRWYZ"
                           "RRRRRYYRRRZWYZRYRYYRYYRRRZWWRYYYRYYYYYRZWWRYYRYRYYYYRRWWRYYYRWR"
                           "YBRRZRRRYRRWWWRYRWZZRYYRRBBWRYRWWZZRYYBBBRRYBWWRZZRYYYRRYYZZWZR"
                           "RWRYYRBYRZZWZZRYBRYYYYYRRZZRWWYYYWWRRRZZZZWRRWWRRRWZZZ";

    void initVars();
    void drawBitmap(char img[], int imgW, int imgH, int x, int y, int scale);
    void drawalien_shooter_score(bool win);
    void gameOver();
    void levelUp();
    int findAlienX(int num);
    int findAlienY(int num);
    int findOldAlienX(int num);
    int findOldAlienY(int num);
    bool alienShot(int num);
    bool onPlayer(int num);
    bool exceedBoundary(int num);
    void moveAliens();
    void fireDaLazer();
    void keepFirinDaLazer(int bulletNo);
    void stopFirinDaLazer(int bulletNo);
    void moveShip();
    void up();
    void down();
    void left();
    void right();
    void select();
    void spaceShoot_run();
};

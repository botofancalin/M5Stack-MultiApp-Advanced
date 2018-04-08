//======================== intro =======================================
//      Space Shooter, basically knock-off Space Invaders
//             && also maybe a bit of Galaga
//   Written by Tyler Edwards for the Hackerbox #0020 badge
//  But should work on any ESP32 && Adafruit ILI9341 display
//        I am sorry for the messy code, you'll just
//                  have to live with it
//      Tyler on GitHub: https://github.com/HailTheBDFL/
//          Hackerboxes: http://hackerboxes.com/
//=========================== setup ===================================
// Space Shooter with M5STACK : 2018.01.12 Transplant by macsbug
// Controller   : Buttons A = LEFT, B = RIGHT, C = START || SHOOTING
// Github:https://macsbug.wordpress.com/2018/01/12/esp32-spaceshooter-with-m5stack/
//===================================================================
#include "../../apps.h"
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
char shipImg[] = "ZZZZZZWWZZZZZZZZZZZYWWYZZZZZZZZZZWWWWZZZZZZZZZZ"
                 "WWWWZZZZZZZZZWWWWWWZZZZZZZZWWWWWWZZZZZYZZWWWWWWZZYZZYZZWWWWWWZZ"
                 "YZWWZZWWBBWWZZWWWWZZWBBBBWZZWWWWZWWBBBBWWZWWWWZWWWWWWWWZWWWWWWW"
                 "WWWWWWWWWWRWWWWWWWWWWRWZZWWWWWWWWWWZZZZZWRRWWRRWZZZ";
//flames
const int flamesImgW = 12;
const int flamesImgH = 6;
char flamesImg[] = "RZZZZZZZZZZRRZRYYRRYYRZRRZRYYRRYYRZRZZRYRZZRY"
                   "RZZZZZRZZZZRZZZZZZRZZZZRZZZ";
//alien
const int alienImgW = 14;
const int alienImgH = 11;
char alienImg[] = "GGGZZZZZZZZGGGZZZGZZZZZZGZZZZZGGGGGGGGGGZZZGGG"
                  "GGGGGGGGGZGGGZGGGGGGZGGGGGGZZGGGGZZGGGGGGGGGGGGGGGGGGGGGGGGGGGG"
                  "GGGGZZZGGZZGGZZZGZGGZGGZZGGZGGZZZZZZGZZGZZZZZ";
//ship 'sploded
const int splodedImgW = 14;
const int splodedImgH = 16;
char splodedImg[] = "ZZZZZZWWZZZZZZZZZZRYWWYRZZZYZZZRRWWRRRRZRWYZ"
                    "RRRRRYYRRRZWYZRYRYYRYYRRRZWWRYYYRYYYYYRZWWRYYRYRYYYYRRWWRYYYRWR"
                    "YBRRZRRRYRRWWWRYRWZZRYYRRBBWRYRWWZZRYYBBBRRYBWWRZZRYYYRRYYZZWZR"
                    "RWRYYRBYRZZWZZRYBRYYYYYRRZZRWWYYYWWRRRZZZZWRRWWRRRWZZZ";
//=============================== setup && loop ==================

void initVars()
{
  //============================= game variables =========================
  offsetM = 0;
  offsetT = 0;
  offsetF = 0;
  offsetB = 0;
  offsetA = 0;
  offsetAF = 0;
  offsetAB = 0;
  offsetS = 0;
  threshold = 40;
  startPrinted = false;
  beginGame = false;
  beginGame2 = true;
  play = false;
  alien_shooter_score = 0;
  alien_shooter_scoreInc = 10;
  level = 1;
  //---------------------Player---------------------------------------
  shipX = 147;
  shipY = 190;
  oldShipX = 0;
  oldShipY = 0;
  changeShipX = 0;
  changeShipY = 0;
  shipSpeed = 50;
  doSplode = false;
  fire = false;
  fFireX[5] = {0};
  fFireY[5] = {0};
  fFireAge[5] = {0};
  //--------------------------Aliens----------------------------------
  alienLive[18];
  alienLiveCount = 18;
  alienX = 7;
  alienY = 25;
  oldAlienX = 7;
  oldAlienY = 25;
  changeAlienX = 6;
  changeAlienY = 0;
  alienSpeed = 200;
  oldAlienSpeed;
  aFireX[5];
  aFireY[5];
  aFireAge[5];
  chanceOfFire = 2;
}
//==================================================================

void drawBitmap(char img[], int imgW, int imgH, int x, int y, int scale)
{
  uint16_t cellColor;
  char curPix;
  for (int i = 0; i < imgW * imgH; i++)
  {
    curPix = img[i];
    if (curPix == 'W')
    {
      cellColor = WHITE;
    }
    else if (curPix == 'Y')
    {
      cellColor = YELLOW;
    }
    else if (curPix == 'B')
    {
      cellColor = BLUE;
    }
    else if (curPix == 'R')
    {
      cellColor = RED;
    }
    else if (curPix == 'G')
    {
      cellColor = 0x5E85;
    }
    if (curPix != 'Z' && scale == 1)
    {
      M5.Lcd.drawPixel(x + i % imgW, y + i / imgW, cellColor);
    }
    else if (curPix != 'Z' && scale > 1)
    {
      M5.Lcd.fillRect(x + scale * (i % imgW), y + scale * (i / imgW), scale, scale, cellColor);
    }
  }
}
//==================================================================
void drawalien_shooter_score(bool win)
{
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
  if (win)
  {
    M5.Lcd.drawString("LEVEL UP!", 53, 40);
  }
  else
  {
    M5.Lcd.drawString("GAME OVER", 53, 40);
  }
  for (; millis() - offsetM <= 1000;)
  M5.Lcd.drawString("Score: " + String(alien_shooter_score), 80, 89);
  offsetM = millis();
  for (; millis() - offsetM <= 1000;)
  {
  }
  M5.Lcd.drawString("Level: " + String(level), 80, 128);
}
// functions =======================================================
void gameOver()
{
  play = false;
  if (doSplode)
  {
    drawBitmap(splodedImg, splodedImgW, splodedImgH, shipX, shipY, 2);
  }
  M5.Lcd.fillScreen(BLACK);
  drawalien_shooter_score(false);
  delay(1000);
  M5.Lcd.drawString("Start", 132, 221);
  M5.Lcd.drawString("Exit", 232, 221);
  while (1)
  {
    // wait for push button
    if (M5.BtnB.wasPressed())
    {
      break;
    }
    if (M5.BtnC.wasPressed())
    {
      return;
    }
    M5.update();
  }
}
//==================================================================
void levelUp()
{
  play = false;
  memset(alienLive, true, 18);
  memset(aFireX, 0, 5);
  memset(aFireY, 0, 5);
  memset(aFireAge, 0, 5);
  alienX = 7;
  alienY = 25;
  oldAlienX = 7;
  oldAlienY = 25;
  alienSpeed = oldAlienSpeed;
  if (alienSpeed > 100)
  {
    alienSpeed -= 10;
    chanceOfFire -= 10;
  }
  else if (alienSpeed > 50)
  {
    alienSpeed -= 10;
    chanceOfFire -= 5;
  }
  else if (alienSpeed > 25)
  {
    alienSpeed -= 5;
    chanceOfFire -= 1;
  }
  alien_shooter_score += 50;
  alien_shooter_scoreInc += 5;
  changeShipX = 0;
  changeShipY = 0;
  for (unsigned long i = millis(); millis() - i <= 1600;)
  {
    if (millis() - offsetM >= 20)
    {
      M5.Lcd.fillRect(oldShipX, oldShipY, 28, 44, BLACK);
      drawBitmap(shipImg, shipImgW, shipImgH, shipX, shipY, 2);
      drawBitmap(flamesImg, flamesImgW, flamesImgH, shipX + 1,
                 shipY + 32, 2);
      oldShipX = shipX;
      oldShipY = shipY;
      shipY -= 6;
      offsetM = millis();
    }
  }
  drawalien_shooter_score(true);
  level += 1;
  shipX = 147;
  shipY = 190;
  for (; millis() - offsetM <= 4000;)
  {
  }
  M5.Lcd.fillScreen(BLACK);
  offsetM = millis();
  play = true;
}
//==================================================================
int findAlienX(int num) { return alienX + 42 * (num % 6); }
//==================================================================
int findAlienY(int num) { return alienY + 33 * (num / 6); }
//==================================================================
int findOldAlienX(int num) { return oldAlienX + 42 * (num % 6); }
//==================================================================
int findOldAlienY(int num) { return oldAlienY + 33 * (num / 6); }
//==================================================================
bool alienShot(int num)
{
  for (int i; i < 5; i++)
  {
    if (fFireAge[i] < 20 && fFireAge[i] > 0)
    {
      if (fFireX[i] > findAlienX(num) - 4 && fFireX[i] < findAlienX(num) + 28 && fFireY[i] < findAlienY(num) + 22 && fFireY[i] > findAlienY(num) + 4)
      {
        fFireAge[i] = 20;
        return true;
      }
    }
  }
  return false;
}
//==================================================================
bool onPlayer(int num)
{
  if (findAlienX(num) - shipX < 24 && findAlienX(num) - shipX > -28 && findAlienY(num) - shipY < 32 &&
      findAlienY(num) - shipY > -22)
  {
    doSplode = true;
    return true;
  }
  else
  {
    return false;
  }
}
//==================================================================
bool exceedBoundary(int num)
{
  if (findAlienY(num) > 218)
  {
    return true;
  }
  else
  {
    return false;
  }
}
//==================================================================
void moveAliens()
{
  for (int i = 0; i < 18; i++)
  {
    if (alienLive[i])
    {
      M5.Lcd.fillRect(findOldAlienX(i), findOldAlienY(i), 28, 22, BLACK);
      drawBitmap(alienImg, alienImgW, alienImgH, findAlienX(i),
                 findAlienY(i), 2);
    }
  }
  oldAlienX = alienX;
  oldAlienY = alienY;
  alienX += changeAlienX;
  alienY += changeAlienY;
  if (changeAlienY != 0)
  {
    changeAlienY = 0;
  }
}
//---------------------------Player---------------------------------
void fireDaLazer()
{
  int bulletNo = -1;
  for (int i = 0; i < 4; i++)
  {
    if (fFireAge[i] == 0)
    {
      bulletNo = i;
    }
  }
  if (bulletNo != -1)
  {
    fFireAge[bulletNo] = 1;
    fFireX[bulletNo] = shipX + 13;
    fFireY[bulletNo] = shipY - 4;
    M5.Lcd.fillRect(fFireX[bulletNo], fFireY[bulletNo], 4, 3, MAGENTA);
  }
  fire = false;
}
//==================================================================
void keepFirinDaLazer(int bulletNo)
{
  M5.Lcd.fillRect(fFireX[bulletNo], fFireY[bulletNo], 4, 4, BLACK);
  fFireY[bulletNo] -= 8;
  M5.Lcd.fillRect(fFireX[bulletNo], fFireY[bulletNo], 4, 4, MAGENTA);
  fFireAge[bulletNo] += 1;
}
//==================================================================
void stopFirinDaLazer(int bulletNo)
{
  M5.Lcd.fillRect(fFireX[bulletNo], fFireY[bulletNo], 4, 4, BLACK);
  fFireAge[bulletNo] = 0;
}
//==================================================================
void moveShip()
{
  if (shipX + changeShipX < 288 && shipX + changeShipX > 6 && changeShipX != 0)
  {
    shipX += changeShipX;
  }
  if (shipY + changeShipY > 24 && shipY + changeShipY < 192 && changeShipY != 0)
  {
    shipY += changeShipY;
  }
  if (oldShipX != shipX || oldShipY != shipY)
  {
    M5.Lcd.fillRect(oldShipX, oldShipY, 28, 44, BLACK);
    oldShipX = shipX;
    oldShipY = shipY;
    drawBitmap(shipImg, shipImgW, shipImgH, shipX, shipY, 2);
  }
}
//=========================== button functions =====================
void up()
{
  if (millis() - offsetT >= 50 && play)
  {
    changeShipX = 0;
    changeShipY = -6;
    offsetT = millis();
  }
}
//==================================================================
void down()
{
  if (millis() - offsetT >= 50 && play)
  {
    changeShipX = 0;
    changeShipY = 6;
    offsetT = millis();
  }
}
//==================================================================
void left()
{
  if (millis() - offsetT >= 50 && play)
  {
    changeShipX = -6;
    changeShipY = 0;
    offsetT = millis();
  }
}
//==================================================================
void right()
{
  if (millis() - offsetT >= 50 && play)
  {
    changeShipX = 6;
    changeShipY = 0;
    offsetT = millis();
  }
}
//==================================================================
void select()
{
  if (millis() - offsetF >= 500 && play)
  {
    fire = true;
    offsetF = millis();
  }
  if (!beginGame)
  {
    beginGame = true;
  }
}

//==================================================================
void spaceShoot_run()
{
	initVars();
	memset(alienLive, true, 18);
	memset(aFireX, 0, 5);
	memset(aFireY, 0, 5);
	memset(aFireAge, 0, 5);
	M5.Lcd.fillScreen(BLACK);
	M5.Lcd.setTextColor(0x5E85);
	M5.Lcd.setTextSize(3);
	randomSeed(analogRead(A0));
  while (1)
  {
    if (M5.BtnA.isPressed())
    {
      left();
    }
    if (M5.BtnC.isPressed())
    {
      right();
    }
    if (M5.BtnB.isPressed())
    {
      select();
    }
    //-------------Start Screen--------------
    if (millis() - offsetS >= 900 && !beginGame)
    {
      if (!startPrinted)
      {
        M5.Lcd.drawString(">START<", 77, 105);
        startPrinted = true;
        offsetS = millis();
      }
      else
      {
        M5.Lcd.fillRect(77, 105, 244, 32, BLACK);
        startPrinted = false;
        offsetS = millis();
      }
    }
    if (beginGame && beginGame2)
    {
      M5.Lcd.fillRect(77, 105, 244, 32, BLACK);
      beginGame2 = false;
      play = true;
    }
    //-------------Player-----------------------------------------------
    if (millis() - offsetM >= shipSpeed && play)
    {
      moveShip();
      offsetM = millis();
    }
    if (oldShipX != shipX || oldShipY != shipY)
    {
      M5.Lcd.fillRect(oldShipX, oldShipY, 28, 44, BLACK);
      oldShipX = shipX;
      oldShipY = shipY;
      drawBitmap(shipImg, shipImgW, shipImgH, shipX, shipY, 2);
    }
    if (fire && play)
    {
      fireDaLazer();
    }
    if (millis() - offsetB >= 50)
    {
      for (int i = 0; i < 5; i++)
      {
        if (fFireAge[i] < 20 && fFireAge[i] > 0)
        {
          keepFirinDaLazer(i);
        }
        if (fFireAge[i] == 20)
        {
          stopFirinDaLazer(i);
        }
      }
      offsetB = millis();
    }
    if (millis() - offsetT > 50)
    {
      changeShipX = 0;
      changeShipY = 0;
    }
    //---------------Aliens--------------------------------------------
    if (millis() - offsetA >= alienSpeed && play)
    {
      moveAliens();
      offsetA = millis();
    }
    if (findAlienX(5) >= 294)
    {
      changeAlienX = -3;
      changeAlienY = 7;
    }
    if (alienX <= 6)
    {
      changeAlienX = 3;
      changeAlienY = 7;
    }
    alienLiveCount = 0;
    for (int i = 0; i < 18; i++)
    {
      if (alienLive[i])
      {
        alienLiveCount += 1;
        if (alienShot(i))
        {
          M5.Lcd.fillRect(findOldAlienX(i), findOldAlienY(i), 28, 22, BLACK);
          alienLiveCount -= 1;
          alienLive[i] = false;
          alien_shooter_score += alien_shooter_scoreInc;
        }
        if (onPlayer(i) || exceedBoundary(i))
        {
          return;
        }
      }
    }
    if (alienLiveCount == 1)
    {
      oldAlienSpeed = alienSpeed;
      if (alienSpeed > 50)
      {
        alienSpeed -= 10;
      }
      else
      {
        alienSpeed = 20;
      }
    }
    if (alienLiveCount == 0)
    {
      levelUp();
    }
    M5.update();
  }
}

void spaceShootest_run()
{
  spaceShoot_run();
  gameOver();
}
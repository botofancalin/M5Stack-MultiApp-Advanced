#pragma once
#include "M5Stack.h"

class Mp3PlayerClass
{
  public:
    Mp3PlayerClass();
    ~Mp3PlayerClass();

    void Play(String *fileName);

  private:
    unsigned long currentMillis;
    unsigned long genSpectrum_previousMillis = 0;
    unsigned long drawTimeline_previousMillis = 0;
    int x = 30;
    int y = 180;
    int width = 200;
    int heightLine = 3;
    int heightMark = 20;
    int widthMark = 3;
    int yClear, wClear, size_, pos_, oldpos_ = 0, xPos;
    int xs = 12;
    int ys = 60;
    int padding = 20;
    int height = 70;
    int widthS = 25;
    int aH, aY, bH, bY, bX, cH, cY, cX, dH, dY, dX, eH, eY, eX, fH, fY, fX, gH, gY, gX;

    void getvolume();
    void setVolume(int *v);
    void drawSpectrum(int a, int b, int c, int d, int e, int f, int g);
    void genSpectrum();
    void drawTimeline();

    AudioGeneratorMP3 *mp3;
    AudioFileSourceSD *file;
    AudioOutputI2S *out;
};

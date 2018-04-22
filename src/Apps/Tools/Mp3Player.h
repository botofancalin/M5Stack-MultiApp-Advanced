#ifndef mp3player_h
#define mp3player_h

#include "M5StackSAM.h"
#include "AudioFileSourceSD.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

class Mp3PlayerClass
{
public:
    Mp3PlayerClass();
    ~Mp3PlayerClass();

    void Play(String *fileName);

private:
unsigned long genSpectrum_previousMillis = 0;

void getvolume();
void setVolume(int *v);
void drawSpectrum(int a, int b, int c, int d, int e, int f, int g);
void genSpectrum();
void drawTimeline();

AudioGeneratorMP3 *mp3;
AudioFileSourceSD *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;
    
};


#endif


#include "../../apps.h"

AudioGeneratorMP3 *mp3;
AudioFileSourceSD *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;

float vol;

void getvolume()
{
    preferences.begin("Mp3-Volume", false);
    vol = preferences.getFloat("volume", 15.0f);
    preferences.end();
}

void setVolume(float *v)
{
    float volume = *v / 100.0f;
    out->SetGain(volume);
}

unsigned long drawTimeline_previousMillis = 0;
void drawTimeline()
{
  unsigned long currentMillis = millis();
  if (currentMillis - drawTimeline_previousMillis > 250)
  {
    int x = 30;
    int y = 180;
    int width = 260;
    int heightLine = 3;
    int heightMark = 20;
    int widthMark = 3;
    int yClear = y - (heightMark / 2);
    int wClear = width + (widthMark / 2);

    drawTimeline_previousMillis = currentMillis;
    M5.Lcd.fillRect(x, yClear +1, wClear, heightMark, 0);
    M5.Lcd.fillRect(x, y, width, heightLine, 0x7bef);
    int size_ = id3->getSize();
    int pos_ = id3->getPos();
    int xPos = x + ((pos_ * (width - (widthMark / 2))) / size_);
    M5.Lcd.fillRect(xPos, yClear+1, widthMark, heightMark, 0xe8e4);
  }
}

void mp3Player(String *fileName)
{
    M5.update();
    MyMenu.drawAppMenu(F("Mp3Player"), F("VOL-"), F("EXIT"), F("VOL+"));
    MyMenu.windowClr();
    getvolume();
    file = new AudioFileSourceSD((*fileName).c_str());
    id3 = new AudioFileSourceID3(file);
    out = new AudioOutputI2S(0, 1);
    out->SetChannels(2);
    mp3 = new AudioGeneratorMP3();
    mp3->begin(id3, out);
    setVolume(&vol);

    while (!M5.BtnB.wasPressed())
    {
        if (mp3->isRunning())
        {
            if (!mp3->loop())
            {
                mp3->stop();
                break;
            }
            drawTimeline();
        }
        if (M5.BtnA.wasPressed() && vol > 0)
        {
            vol -= 5;
            setVolume(&vol);
        }
        if (M5.BtnC.wasPressed() && vol < 100)
        {
            vol += 5;
            setVolume(&vol);
        }
        M5.update();
    }
    preferences.begin("Mp3-Volume", false);
    preferences.putFloat("volume", vol);
    preferences.end();
    mp3->stop();
    out->stop();
    id3->close();
    file->close();
    delete mp3;
    delete out;
    delete file;
    mp3 = NULL;
    file = NULL;
    out = NULL;
    dacWrite(25,0);
    dacWrite(26,0);
    MyMenu.drawAppMenu(F("SD BROWSER"), F("EXIT"), F("OPEN"), F(">"));
    MyMenu.showList();
}
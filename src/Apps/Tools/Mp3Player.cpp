#include "../../apps.h"

AudioGeneratorMP3 *mp3;
AudioFileSourceSD *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;

int vol;
int old_vol;

void getvolume()
{
    preferences.begin("Mp3-Volume", false);
    vol = preferences.getFloat("volume", 15.0f);
    preferences.end();
}

void setVolume(int *v)
{
    float volume = *v / 33.3f; // volme max value can be 3.99
    out->SetGain(volume);
}

void drawSpectrum(int a, int b, int c, int d, int e, int f, int g)
{
    int x = 12;
    int y = 60;
    int padding = 20;
    int height = 70;
    int width = 25;

    int aH = ((a * height) / 100);
    int aY = y + (height - aH);
    M5.Lcd.fillRect(x, y, width, height, 0);
    M5.Lcd.fillRect(x, aY, width, aH, LIGHTGREY); //0xe8e4

    int bH = ((b * height) / 100);
    int bY = y + (height - bH);
    int bX = x + width + padding;
    M5.Lcd.fillRect(bX, y, width, height, 0);
    M5.Lcd.fillRect(bX, bY, width, bH, LIGHTGREY); //0xff80

    int cH = ((c * height) / 100);
    int cY = y + (height - cH);
    int cX = bX + width + padding;
    M5.Lcd.fillRect(cX, y, width, height, 0);
    M5.Lcd.fillRect(cX, cY, width, cH, LIGHTGREY); //0x2589

    int dH = ((d * height) / 100);
    int dY = y + (height - dH);
    int dX = cX + width + padding;
    M5.Lcd.fillRect(dX, y, width, height, 0);
    M5.Lcd.fillRect(dX, dY, width, dH, LIGHTGREY); //0x051d

    int eH = ((e * height) / 100);
    int eY = y + (height - eH);
    int eX = dX + width + padding;
    M5.Lcd.fillRect(eX, y, width, height, 0);
    M5.Lcd.fillRect(eX, eY, width, eH, LIGHTGREY); //0x051d

    int fH = ((f * height) / 100);
    int fY = y + (height - fH);
    int fX = eX + width + padding;
    M5.Lcd.fillRect(fX, y, width, height, 0);
    M5.Lcd.fillRect(fX, fY, width, fH, LIGHTGREY); //0x051d

    int gH = ((g * height) / 100);
    int gY = y + (height - gH);
    int gX = fX + width + padding;
    M5.Lcd.fillRect(gX, y, width, height, 0);
    M5.Lcd.fillRect(gX, gY, width, gH, LIGHTGREY); //0x051d
}

unsigned long genSpectrum_previousMillis = 0;
void genSpectrum()
{
    unsigned long currentMillis = millis();
    if (currentMillis - genSpectrum_previousMillis > 100)
    {
        genSpectrum_previousMillis = currentMillis;
        drawSpectrum(random(0, 101), random(0, 101), random(0, 101), random(0, 101), random(0, 101), random(0, 101), random(0, 101));
    }
}

unsigned long drawTimeline_previousMillis = 0;
void drawTimeline()
{
    unsigned long currentMillis = millis();
    if (currentMillis - drawTimeline_previousMillis > 250)
    {
        int x = 30;
        int y = 180;
        int width = 200;
        int heightLine = 3;
        int heightMark = 20;
        int widthMark = 3;
        int yClear = y - (heightMark / 2);
        int wClear = width + (widthMark / 2);

        drawTimeline_previousMillis = currentMillis;
        M5.Lcd.fillRect(x, yClear + 2, wClear, heightMark, 0);
        M5.Lcd.fillRect(x, y, width, heightLine, 31727);
        int size_ = id3->getSize();
        int pos_ = id3->getPos();
        int xPos = x + ((pos_ * (width - (widthMark / 2))) / size_);
        M5.Lcd.fillRect(xPos, yClear + 2, widthMark, heightMark, 59620);
        M5.Lcd.fillRect(width + 50, y - 4, 70, 10, BLACK);
        M5.Lcd.drawNumber((pos_), width + 50, y - 7, 2);
    }
}

void mp3Player(String *fileName)
{
    M5.update();
    MyMenu.drawAppMenu(F("Mp3Player"), F("VOL-"), F("EXIT"), F("VOL+"));
    MyMenu.windowClr();
    M5.Lcd.setTextColor(CYAN);
    M5.Lcd.drawCentreString(*fileName, 158, 140, 2);
    M5.Lcd.setTextColor(WHITE);
    getvolume();
    file = new AudioFileSourceSD((*fileName).c_str());
    id3 = new AudioFileSourceID3(file);
    out = new AudioOutputI2S(0, 1);
    out->SetChannels(2);
    mp3 = new AudioGeneratorMP3();
    mp3->begin(id3, out);
    setVolume(&vol);
    old_vol = static_cast<int>(vol);
    M5.Lcd.setTextColor(ORANGE);
    M5.Lcd.drawCentreString("Volume: " + String(vol), 158, 190, 2);
    M5.Lcd.setTextColor(WHITE);

    while (!M5.BtnB.wasPressed())
    {
        if (mp3->isRunning())
        {
            if (!mp3->loop())
            {
                mp3->stop();
                break;
            }
            genSpectrum();
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
        if (vol != old_vol)
        {
            M5.Lcd.setTextColor(ORANGE);
            M5.Lcd.fillRect(120, 190, 80, 14, BLACK);
            M5.Lcd.drawCentreString("Volume: " + String(vol), 158, 190, 2);
            M5.Lcd.setTextColor(WHITE);
            old_vol = vol;
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
    dacWrite(25, 0);
    dacWrite(26, 0);
    MyMenu.windowClr();
    MyMenu.drawAppMenu(F("SD BROWSER"), F("EXIT"), F("OPEN"), F(">"));
    MyMenu.showList();
}
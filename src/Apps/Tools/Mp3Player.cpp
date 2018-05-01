#include "Mp3Player.h"

void Mp3PlayerClass::getvolume()
{
    preferences.begin("Volume", false);
    vol = preferences.getFloat("vol", 15.0f);
    preferences.end();
}

void Mp3PlayerClass::setVolume(int *v)
{
    float volume = *v / 71.4f; // volme max value can be 3.99
    out->SetGain(volume);
}

void Mp3PlayerClass::drawSpectrum(int a, int b, int c, int d, int e, int f, int g)
{
    aH = ((a * height) / 100);
    aY = ys + (height - aH);
    M5.Lcd.fillRect(xs, ys, widthS, height, 0);
    M5.Lcd.fillRect(xs, aY, widthS, aH, LIGHTGREY); //0xe8e4

    bH = ((b * height) / 100);
    bY = ys + (height - bH);
    bX = xs + widthS + padding;
    M5.Lcd.fillRect(bX, ys, widthS, height, 0);
    M5.Lcd.fillRect(bX, bY, widthS, bH, LIGHTGREY); //0xff80

    cH = ((c * height) / 100);
    cY = ys + (height - cH);
    cX = bX + widthS + padding;
    M5.Lcd.fillRect(cX, ys, widthS, height, 0);
    M5.Lcd.fillRect(cX, cY, widthS, cH, LIGHTGREY); //0x2589

    dH = ((d * height) / 100);
    dY = ys + (height - dH);
    dX = cX + widthS + padding;
    M5.Lcd.fillRect(dX, ys, widthS, height, 0);
    M5.Lcd.fillRect(dX, dY, widthS, dH, LIGHTGREY); //0x051d

    eH = ((e * height) / 100);
    eY = ys + (height - eH);
    eX = dX + widthS + padding;
    M5.Lcd.fillRect(eX, ys, widthS, height, 0);
    M5.Lcd.fillRect(eX, eY, widthS, eH, LIGHTGREY); //0x051d

    fH = ((f * height) / 100);
    fY = ys + (height - fH);
    fX = eX + widthS + padding;
    M5.Lcd.fillRect(fX, ys, widthS, height, 0);
    M5.Lcd.fillRect(fX, fY, widthS, fH, LIGHTGREY); //0x051d

    gH = ((g * height) / 100);
    gY = ys + (height - gH);
    gX = fX + widthS + padding;
    M5.Lcd.fillRect(gX, ys, widthS, height, 0);
    M5.Lcd.fillRect(gX, gY, widthS, gH, LIGHTGREY); //0x051d
}

void Mp3PlayerClass::genSpectrum()
{
    currentMillis = millis();
    if (currentMillis - genSpectrum_previousMillis > 100)
    {
        genSpectrum_previousMillis = currentMillis;
        drawSpectrum(random(0, 101), random(0, 101), random(0, 101), random(0, 101), random(0, 101), random(0, 101), random(0, 101));
    }
}

void Mp3PlayerClass::drawTimeline()
{
    currentMillis = millis();
    if (currentMillis - drawTimeline_previousMillis > 250)
    {
        yClear = y - (heightMark / 2);
        wClear = width + (widthMark / 2);

        drawTimeline_previousMillis = currentMillis;
        M5.Lcd.fillRect(x, yClear + 2, wClear, heightMark, 0);
        M5.Lcd.fillRect(x, y, width, heightLine, 31727);
        size_ = file->getSize();
        pos_ = file->getPos();
        xPos = x + ((pos_ * (width - (widthMark / 2))) / size_);
        M5.Lcd.fillRect(xPos, yClear + 2, widthMark, heightMark, 59620);
        M5.Lcd.setTextColor(BLACK);
        M5.Lcd.drawNumber((oldpos_), width + 50, y - 7, 2);
        M5.Lcd.setTextColor(WHITE);
        M5.Lcd.drawNumber((pos_), width + 50, y - 7, 2);
        oldpos_ = pos_;
    }
}

void Mp3PlayerClass::Play(String *fileName)
{
    MyMenu.windowClr();
    M5.Lcd.setTextColor(CYAN);
    M5.Lcd.drawCentreString(*fileName, 158, 140, 2);
    M5.Lcd.setTextColor(WHITE);
    getvolume();
    file = new AudioFileSourceSD((*fileName).c_str());
    out = new AudioOutputI2S(0, 1);
    mp3 = new AudioGeneratorMP3();
    out->SetChannels(2);
    mp3->begin(file, out);
    setVolume(&vol);
    old_vol = vol;
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
    preferences.begin("Volume", false);
    preferences.putFloat("vol", vol);
    preferences.end();
    mp3->stop();
    out->stop();
    file->close();
    mp3 = NULL;
    out = NULL;
    file = NULL;
    delete mp3;
    delete out;
    delete file;
    dacWrite(25, 0);
    dacWrite(26, 0);
    MyMenu.windowClr();
}

Mp3PlayerClass::Mp3PlayerClass()
{
    M5.update();
    MyMenu.drawAppMenu(F("Mp3Player"), F("VOL-"), F("EXIT"), F("VOL+"));
}

Mp3PlayerClass::~Mp3PlayerClass()
{
    MyMenu.drawAppMenu(F("SD BROWSER"), F("EXIT"), F("OPEN"), F(">"));
    MyMenu.showList();
}
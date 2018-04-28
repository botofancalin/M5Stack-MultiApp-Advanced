#include "WebRadio.h"
#define MYFONT &FreeSerif12pt7b

void WebRadioClass::getvolume()
{
	preferences.begin("Volume", false);
	vol = preferences.getFloat("vol", 15.0f);
	preferences.end();
}

void WebRadioClass::setVolume(int *v)
{
	float volume = *v / 71.4f; // volme max value can be 3.99
	out->SetGain(volume);
}
String _s2, _s3;
// Called when a metadata event occurs (i.e. an ID3 tag, an ICY block, etc.
void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string)
{
	String s1 = string;
	String s2 = strstr(s1.c_str(), "/");
	String s3 = s1;
	s3.replace(s2, "");
	s2.replace("/", "");
	M5.Lcd.setTextColor(BLACK);
	M5.Lcd.setFreeFont(MYFONT);
	M5.Lcd.drawCentreString(_s3, 160, 35, 1);
	M5.Lcd.drawCentreString(_s2, 160, 60, 1);
	M5.Lcd.setTextColor(ORANGE);
	M5.Lcd.drawCentreString(s3, 160, 35, 1);
	M5.Lcd.drawCentreString(s2, 160, 60, 1);
	_s2 = s2;
	_s3 = s3;
}

// Called when there's a warning or error (like a buffer underflow or decode hiccup)
String _e1;
void StatusCallback(void *cbData, int code, const char *string)
{
	String e1 = string;
	const char *ptr = reinterpret_cast<const char *>(cbData);
	M5.Lcd.setTextColor(BLACK);
	M5.Lcd.drawCentreString(_e1, 160, 140, 2);

	M5.Lcd.setTextColor(WHITE);
	M5.Lcd.drawCentreString(e1, 160, 140, 2);
	_e1 = e1;
}

void WebRadioClass::Run()
{
	M5.update();
	getvolume();
	MyMenu.drawAppMenu(F("WebRadio"), F("Vol-"), F("Stop"), F("Vol+"));
	M5.Lcd.setTextColor(ORANGE);
	M5.Lcd.drawCentreString("Volume: " + String(vol), 158, 190, 2);
	M5.Lcd.setTextColor(WHITE);

	if (WiFi.isConnected())
	{
		while (play)
		{
			unsigned long now = millis();
			if (now - lastcheck >= 1000)
			{

				M5.Lcd.setTextColor(WHITE, 15);
				SignalStrength = map(100 + WiFi.RSSI(), 5, 90, 0, 100);
				M5.Lcd.drawRightString("WiFi: " + String(SignalStrength) + " %", 310, 5, 2);
				lastcheck = now;
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
			if (M5.BtnB.pressedFor(2000))
			{
				play = !play;
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

			if (upd)
			{
				preallocateBuffer = malloc(preallocateBufferSize);
				file = new AudioFileSourceICYStream(URL);
				file->RegisterMetadataCB(MDCallback, (void *)"ICY");
				buff = new AudioFileSourceBuffer(file, preallocateBuffer, preallocateBufferSize);
				buff->RegisterStatusCB(StatusCallback, (void *)"buffer");
				out = new AudioOutputI2S(0, 1);
				mp3 = new AudioGeneratorMP3();
				mp3->begin(buff, out);
				setVolume(&vol);
				old_vol = vol;
				upd = false;
			}
			else
			{
				mp3->loop();
			}
		}
		preferences.begin("Volume", false);
		preferences.putFloat("vol", vol);
		preferences.end();
		free(preallocateBuffer);
		mp3->stop();
		out->stop();
		buff->close();
		file->close();
		mp3 = NULL;
		file = NULL;
		out = NULL;
		buff = NULL;
		delete mp3;
		delete out;
		delete file;
		delete buff;
		dacWrite(25, 0);
		dacWrite(26, 0);
	}
	else
	{
		M5.Lcd.setTextColor(WHITE);
		M5.Lcd.drawCentreString("Wifi Not Connected!", 160, 60, 2);
		delay(3000);
	}
}

WebRadioClass::WebRadioClass()
{
}

WebRadioClass::~WebRadioClass()
{
	MyMenu.show();
}
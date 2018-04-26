#include "WebRadio.h"

void WebRadioClass::getvolume()
{
	preferences.begin("Mp3-Volume", false);
	vol = preferences.getFloat("volume", 15.0f);
	preferences.end();
}

void WebRadioClass::setVolume(int *v)
{
	float volume = *v / 49.9f; // volme max value can be 3.99
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
	M5.Lcd.drawCentreString(_s3, 160, 30, 4);
	M5.Lcd.drawCentreString(_s2, 160, 60, 4);
	M5.Lcd.setTextColor(YELLOW);
	M5.Lcd.drawCentreString(s3, 160, 30, 4);
	M5.Lcd.drawCentreString(s2, 160, 60, 4);
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
	MyMenu.drawAppMenu(F("WebRadio"), F("Vol-"), F("Stop"), F("Vol+"));
	M5.Lcd.setTextColor(ORANGE);
	M5.Lcd.drawCentreString("Volume: " + String(vol), 158, 190, 2);
	M5.Lcd.setTextColor(WHITE);
	
	while (play)
	{
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
		if (M5.BtnB.wasPressed())
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
			//preallocateCodec = malloc(preallocateCodecSize);

			file = new AudioFileSourceICYStream(URL);
			file->RegisterMetadataCB(MDCallback, (void *)"ICY");
			buff = new AudioFileSourceBuffer(file, preallocateBuffer, preallocateBufferSize);
			buff->RegisterStatusCB(StatusCallback, (void *)"buffer");
			out = new AudioOutputI2S(0, true);
			mp3 = new AudioGeneratorMP3();
			//mp3->RegisterStatusCB(StatusCallback, (void *)"mp3");
			mp3->begin(buff, out);
			setVolume(&vol);
			old_vol = vol;
			upd = false;
		}
		else
		{
			mp3->loop();
		}
		//Serial.println(buff->getFillLevel());
		//Serial.println(buff->getPos());
	}
}

WebRadioClass::WebRadioClass()
{
}

WebRadioClass::~WebRadioClass()
{
	preferences.begin("Mp3-Volume", false);
	preferences.putFloat("volume", vol);
	preferences.end();
	free(preallocateBuffer);
	//free(preallocateCodec);
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
	MyMenu.show();
}
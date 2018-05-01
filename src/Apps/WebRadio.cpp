#include "WebRadio.h"
#define MDFONT &FreeSerif12pt7b
#define STFONT &FreeSerif18pt7b

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

void WebRadioClass::StopPlaying()
{
	if (player)
	{
		player->stop();
		delete player;
		player = NULL;
	}
	if (buff)
	{
		buff->close();
		delete buff;
		buff = NULL;
	}
	if (file)
	{
		file->close();
		delete file;
		file = NULL;
	}
}

// Called when a metadata event occurs (i.e. an ID3 tag, an ICY block, etc.
String _s2, _s3;
void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string)
{
	String s1 = string;
	String s2 = strstr(s1.c_str(), "/");
	String s3 = s1;
	s3.replace(s2, "");
	s2.replace("/", "");
	M5.Lcd.setTextColor(BLACK);
	M5.Lcd.setFreeFont(MDFONT);
	M5.Lcd.drawCentreString(_s3, 160, 80, 1);
	M5.Lcd.drawCentreString(_s2, 160, 105, 1);
	M5.Lcd.setTextColor(ORANGE);
	M5.Lcd.drawCentreString(s3, 160, 80, 1);
	M5.Lcd.drawCentreString(s2, 160, 105, 1);
	_s2 = s2;
	_s3 = s3;
}

bool WebRadioClass::GetStations(fs::FS &fs, const char *path)
{
	File sdfile = fs.open(path);
	if (!sdfile)
	{
		return false;
	}

	if (sdfile.available())
	{
		StationList = sdfile.readStringUntil(EOF);
		while (StationList != "\0")
		{
			int t = StationList.indexOf("=");
			name = StationList.substring(0, t);
			StationList.replace(String(name + "="), "");
			int r = StationList.indexOf(";");
			link = StationList.substring(0, r);
			StationList.replace(String(link + ";"), "");
			if (StationList != "\0")
			{
				StationList = StationList.substring(2);
			}
			Name.push_back(name);
			Link.push_back(link);
		}
	}
	sdfile.close();
	return true;
}

void WebRadioClass::Run()
{
	M5.update();
	getvolume();
	M5.drawAppMenu(F("WebRadio"), F("Vol-"), F("Next"), F("Vol+"));
	M5.Lcd.setTextColor(ORANGE);
	M5.Lcd.drawCentreString("Long press 'NEXT' to Exit", 158, 190, 2);
	preallocateBuffer = malloc(preallocateBufferSize);
	preallocateCodec = malloc(preallocateCodecSize);
	out = new AudioOutputI2S(0, 1);

	if (GetStations(SD, "/RadioStations.txt"))
	{
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
				if (M5.BtnB.wasPressed())
				{
					StopPlaying();
					if (Station < (unsigned int)(Link.size() - 1))
					{
						Station++;
					}
					else
					{
						Station = 0;
					}
					upd = true;
				}
				if (M5.BtnB.pressedFor(1000))
				{
					play = false;
				}
				if (vol != old_vol)
				{
					M5.Lcd.HprogressBar(80, 170, 200, 15, GREEN, vol, true);
					old_vol = vol;
				}
				M5.update();

				if (upd)
				{
					M5.Lcd.setTextColor(RED);
					M5.Lcd.drawString("Buffer %", 15, 148, 2);
					M5.Lcd.HprogressBar(80, 150, 200, 15, RED, 0, true);
					rawFillLvl = 0;
					M5.Lcd.setTextColor(GREEN);
					M5.Lcd.drawString("Volume", 15, 168, 2);
					M5.Lcd.HprogressBar(80, 170, 200, 15, GREEN, vol, true);
					M5.Lcd.setFreeFont(STFONT);
					M5.Lcd.setTextColor(BLACK);
					M5.Lcd.drawCentreString(old_Station, 160, 35, 1);
					M5.Lcd.setTextColor(PINK);
					M5.Lcd.drawCentreString(Name[Station], 160, 35, 1);
					old_Station = Name[Station];
					file = new AudioFileSourceICYStream(Link[Station].c_str());
					file->RegisterMetadataCB(MDCallback, (void *)"ICY");
					buff = new AudioFileSourceBuffer(file, preallocateBuffer, preallocateBufferSize);
					player = new AudioGeneratorMP3(preallocateCodec, preallocateCodecSize);
					player->begin(buff, out);
					setVolume(&vol);
					old_vol = vol;
					upd = false;
				}
				else
				{
					player->loop();
					if (rawFillLvl != buff->getFillLevel())
					{
						rawFillLvl = buff->getFillLevel();
						fillLvl = map(rawFillLvl, 0, preallocateBufferSize, 0, 100);
						M5.Lcd.HprogressBar(80, 150, 200, 15, RED, fillLvl, true);
					}
					if (!upd && !fillLvl)
					{
						StopPlaying();
						if (Station < (unsigned int)(Link.size() - 1))
						{
							Station++;
							upd = true;
						}
						else
						{
							break;
						}
					}
				}
			}
			preferences.begin("Volume", false);
			preferences.putFloat("vol", vol);
			preferences.end();
			StopPlaying();
			free(preallocateBuffer);
			free(preallocateCodec);
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
	else
	{
		M5.Lcd.setTextColor(WHITE);
		M5.Lcd.drawCentreString("RadioStations.txt Not Found!", 160, 60, 2);
		delay(3000);
	}
}

WebRadioClass::WebRadioClass()
{
}

WebRadioClass::~WebRadioClass()
{
	Name.clear();
	Name.shrink_to_fit();
	Link.clear();
	Link.shrink_to_fit();
	M5.show();
}
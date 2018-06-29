#include "Wrappers.h"
#include "Resources.h"

const int version = 3;
unsigned long lastcheck = 0;
int SignalStrength = 0;
bool OtaRunning = false;

void setup()
{
	M5m.begin();
	dacWrite(25, 0); // Speaker OFF
	preferences.begin("WiFi", false);
	M5m.WiFi_Mode = preferences.getInt("mode", 0);
	preferences.end();
	WiFi.mode(wifi_mode_t(M5m.WiFi_Mode));
	if (M5m.WiFi_Mode > 0)
	{
		WiFi.begin();
	}

	preferences.begin("Brightness", false);
	M5m.Lcd.setBrightness(preferences.getUShort("light", 95));
	preferences.end();

	//The main menu. Add main menu items here
	M5m.addMenuItem(0, "APPLICATIONS", "<", "OK", ">", 1, Apps, appReturn);
	M5m.addMenuItem(0, "SYSTEM", "<", "OK", ">", 2, System, appReturn);
	M5m.addMenuItem(0, "ABOUT", "<", "OK", ">", -1, About, appAbout);
	M5m.addMenuItem(0, "SLEEP/CHARGING", "<", "OK", ">", -1, Sleep, appSleep);

	M5m.addMenuItem(1, "OSCILLOSCOPE", "<", "OK", ">", -1, Oscilloscope, appOscilloscope);
	M5m.addMenuItem(1, "WEBRADIO", "<", "OK", ">", -1, WebRadio, appWebRadio);
	M5m.addMenuItem(1, "WEATHER STATION", "<", "OK", ">", -1, WeatherStation, appWeatherStation);
	M5m.addMenuItem(1, "WEBSERVER", "<", "OK", ">", -1, Webserver, appWebServer);
	M5m.addMenuItem(1, "SD BROWSER", "<", "OK", ">", -1, Browser, appSdBrowser);
	M5m.addMenuItem(1, "TOOLS", "<", "OK", ">", -1, Tools, appListTools);
	M5m.addMenuItem(1, "GAMES", "<", "OK", ">", -1, Games, appGamesList);
	M5m.addMenuItem(1, "RETURN", "<", "OK", ">", 0, Return, appReturn);

	M5m.addMenuItem(2, "SYSTEM INFORMATIONS", "<", "OK", ">", -1, Sysinfo, appSysInfo);
	M5m.addMenuItem(2, "WIFI CONNECTION", "<", "OK", ">", -1, WifiConn, appWiFiSetup);
	M5m.addMenuItem(2, "DISPLAY BACKLIGHT", "<", "OK", ">", -1, Backlight, appCfgbrightness);
	M5m.addMenuItem(2, "RETURN", "<", "OK", ">", 0, Return, appReturn);

	M5m.show();
}

void loop()
{
	unsigned long now = millis();
	if (now - lastcheck >= 1000)
	{
		M5m.WiFi_Mode = WiFi.getMode();
		if (M5m.WiFi_Mode == WIFI_MODE_STA && WiFi.isConnected())
		{
			M5m.Lcd.setTextColor(WHITE, 15);
			SignalStrength = map(100 + WiFi.RSSI(), 5, 90, 0, 100);
			M5m.Lcd.drawRightString("WiFi: " + String(SignalStrength) + " %", 310, 5, 2);
			if (!OtaRunning)
			{
				M5StackServerOta SrververOta(&version);
				SrververOta.checkForNewVersion();
				appOta();
				OtaRunning = true;
			}
		}
		else if (M5m.WiFi_Mode == WIFI_MODE_APSTA)
		{
			M5m.Lcd.setTextColor(WHITE, 15);
			M5m.Lcd.drawRightString("Clients: " + String(WiFi.softAPgetStationNum()), 300, 5, 2);
		}
		else if (M5m.WiFi_Mode == WIFI_MODE_NULL)
		{
			M5m.Lcd.setTextColor(WHITE, 15);
			M5m.Lcd.drawRightString("Wifi OFF", 310, 5, 2);
		}
		lastcheck = now;
	}

	M5m.update();
	if (OtaRunning)
	{
		ArduinoOTA.handle();
	}
	if (M5m.BtnC.wasPressed())
	{
		M5m.up();
	}
	if (M5m.BtnA.wasPressed())
	{
		M5m.down();
	}
	if (M5m.BtnB.wasPressed())
	{
		M5m.execute();
	}
}

#include "apps.h"
#include <Display.h>

unsigned long sleeptime = 0;
int SignalStrength = 0;
unsigned long waiting = 60000;
unsigned long lastcheck = 0;

void setup()
{
	M5.begin();
	Wire.begin();
	dacWrite(25, 0); // Speaker OFF

	preferences.begin("WiFi-mode", false);
	WiFi_Mode = preferences.getInt("mode", 0);
	preferences.end();
	WiFi.mode(wifi_mode_t(WiFi_Mode));
	if (WiFi_Mode == WIFI_MODE_AP || WiFi_Mode == WIFI_MODE_APSTA)
	{
		WiFi.begin();
	}

	preferences.begin("Brightness", false);
	M5.lcd.setBrightness(preferences.getUShort("Brightness", 95));
	preferences.end();

	MyMenu.addMenuItem(0, "APPLICATIONS", "<", "OK", ">", 1, "/Data/Apps.jpg", appReturn);
	MyMenu.addMenuItem(0, "SYSTEM", "<", "OK", ">", 2, "/Data/System.jpg", appReturn);
	MyMenu.addMenuItem(0, "ABOUT", "<", "OK", ">", -1, "/Data/About.jpg", appAbout);

	MyMenu.addMenuItem(1, "OSCILOSCOPE", "<", "OK", ">", -1, "/Data/Oscilloscope.jpg", appOsciloscope);
	MyMenu.addMenuItem(1, "WEBSERVER", "<", "OK", ">", -1, "/Data/WebServer.jpg", appWebServer);
	MyMenu.addMenuItem(1, "STOPWATCH", "<", "OK", ">", -1, "/Data/Stopwatch.jpg", appStopWatch);
	MyMenu.addMenuItem(1, "TOOLS", "<", "OK", ">", -1, "/Data/Tools.jpg", appListTools);
	MyMenu.addMenuItem(1, "GAMES", "<", "OK", ">", -1, "/Data/Games.jpg", appListGames);
	MyMenu.addMenuItem(1, "RETURN", "<", "OK", ">", 0, "/Data/Return.jpg", appReturn);

	MyMenu.addMenuItem(2, "SYSTEM INFORMATIONS", "<", "OK", ">", -1, "/Data/SysInfo.jpg", appSysInfo);
	MyMenu.addMenuItem(2, "WIFI CONNECTION", "<", "OK", ">", -1, "/Data/WiFi.jpg", appWiFiSetup);
	MyMenu.addMenuItem(2, "DISPLAY BACKLIGHT", "<", "OK", ">", -1, "/Data/BackLight.jpg", appCfgBrigthness);
	MyMenu.addMenuItem(2, "SLEEP/CHARGING", "<", "OK", ">", -1, "/Data/Sleep.jpg", appSleep);
	MyMenu.addMenuItem(2, "RETURN", "<", "OK", ">", 0, "/Data/Return.jpg", appReturn);

	MyMenu.show();
}

void loop()
{
	unsigned long now = millis();
	if (now - lastcheck >= 1000)
	{
		if (WiFi_Mode == WIFI_MODE_STA && WiFi.isConnected())
		{
			M5.Lcd.setTextColor(WHITE, 15);
			SignalStrength = map(100 + WiFi.RSSI(), 5, 90, 0, 100);
			M5.Lcd.drawRightString("WiFi: " + String(SignalStrength) + " %", 310, 5, 2);
		}
		if (WiFi_Mode == WIFI_MODE_AP)
		{
			M5.Lcd.setTextColor(WHITE, 15);
			M5.Lcd.drawRightString("Clients: " + String(WiFi.softAPgetStationNum()), 300, 5, 2);
		}
		if (WiFi_Mode == WIFI_MODE_NULL)
		{
			M5.Lcd.setTextColor(WHITE, 15);
			M5.Lcd.drawRightString("Wifi OFF", 310, 5, 2);
		}
		if (!OtaRunning)
		{
			appOta();
			OtaRunning = true;
		}
		ArduinoOTA.handle();
		lastcheck = now;
	}
	M5.update();
	if (M5.BtnC.wasPressed())
	{
		sleeptime = millis() + waiting;
		MyMenu.up();
	}
	if (M5.BtnA.wasPressed())
	{
		sleeptime = millis() + waiting;
		MyMenu.down();
	}
	if (M5.BtnB.wasPressed())
	{
		sleeptime = millis() + waiting;
		MyMenu.execute();
	}
}

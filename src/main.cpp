#include "Wrappers.h"
#include "Resources.h"

const int version = 2;
unsigned long lastcheck = 0;
int SignalStrength = 0;
bool OtaRunning = false;

void setup()
{
	M5.begin();
	dacWrite(25, 0); // Speaker OFF
	preferences.begin("WiFi", false);
	WiFi_Mode = preferences.getInt("mode", 0);
	preferences.end();
	WiFi.mode(wifi_mode_t(WiFi_Mode));
	if (WiFi_Mode > 0)
	{
		WiFi.begin();
	}

	preferences.begin("Brightnes", false);
	M5.lcd.setBrightness(preferences.getUShort("light", 95));
	preferences.end();

	//The main menu. Add main menu items here
	MyMenu.addMenuItem(0, "APPLICATIONS", "<", "OK", ">", 1, Apps1, appReturn);
	MyMenu.addMenuItem(0, "SYSTEM", "<", "OK", ">", 2, System, appReturn);
	MyMenu.addMenuItem(0, "ABOUT", "<", "OK", ">", -1, About, appAbout);
	MyMenu.addMenuItem(0, "SLEEP/CHARGING", "<", "OK", ">", -1, Sleep, appSleep);

	MyMenu.addMenuItem(1, "OSCILOSCOPE", "<", "OK", ">", -1, Oscilloscope, appOscilloscope);
	MyMenu.addMenuItem(1, "WEBRADIO", "<", "OK", ">", -1, WebRadio, appWebRadio);
	MyMenu.addMenuItem(1, "WEBSERVER", "<", "OK", ">", -1, Webserver, appWebServer);
	MyMenu.addMenuItem(1, "SD BROWSER", "<", "OK", ">", -1, Browser, appSdBrowser);
	MyMenu.addMenuItem(1, "TOOLS", "<", "OK", ">", -1, Tools, appListTools);
	MyMenu.addMenuItem(1, "GAMES", "<", "OK", ">", -1, Games, appGamesList);
	MyMenu.addMenuItem(1, "RETURN", "<", "OK", ">", 0, Return, appReturn);

	MyMenu.addMenuItem(2, "SYSTEM INFORMATIONS", "<", "OK", ">", -1, Sysinfo, appSysInfo);
	MyMenu.addMenuItem(2, "WIFI CONNECTION", "<", "OK", ">", -1, WifiConn, appWiFiSetup);
	MyMenu.addMenuItem(2, "DISPLAY BACKLIGHT", "<", "OK", ">", -1, Backlight, appCfgBrigthness);
	MyMenu.addMenuItem(2, "RETURN", "<", "OK", ">", 0, Return, appReturn);

	MyMenu.show();
}

void loop()
{
	unsigned long now = millis();
	if (now - lastcheck >= 1000)
	{
		WiFi_Mode = WiFi.getMode();
		if (WiFi_Mode == WIFI_MODE_STA && WiFi.isConnected())
		{
			M5.Lcd.setTextColor(WHITE, 15);
			SignalStrength = map(100 + WiFi.RSSI(), 5, 90, 0, 100);
			M5.Lcd.drawRightString("WiFi: " + String(SignalStrength) + " %", 310, 5, 2);
			if (!OtaRunning)
			{
				M5StackServerOta SrververOta(&version);
				SrververOta.checkForNewVersion();
				appOta();
				OtaRunning = true;
				Serial.println("Ota function");
			}
		}
		else if (WiFi_Mode == WIFI_MODE_APSTA)
		{
			M5.Lcd.setTextColor(WHITE, 15);
			M5.Lcd.drawRightString("Clients: " + String(WiFi.softAPgetStationNum()), 300, 5, 2);
		}
		else if (WiFi_Mode == WIFI_MODE_NULL)
		{
			M5.Lcd.setTextColor(WHITE, 15);
			M5.Lcd.drawRightString("Wifi OFF", 310, 5, 2);
		}
		lastcheck = now;
	}

	M5.update();
	if (OtaRunning)
	{
		ArduinoOTA.handle();
	}
	if (M5.BtnC.wasPressed())
	{
		MyMenu.up();
	}
	if (M5.BtnA.wasPressed())
	{
		MyMenu.down();
	}
	if (M5.BtnB.wasPressed())
	{
		MyMenu.execute();
	}
}

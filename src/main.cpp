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
	WiFi.mode(WIFI_MODE_STA);
	WiFi.begin();

	if (!EEPROM.begin(EEPROM_SIZE))
	{
		M5.powerOFF();
	}
	else
	{
		M5.lcd.setBrightness(byte(EEPROM.read(0)));
	}

	// CHANGING COLOR SCHEMA:
	//  MyMenu.setColorSchema(MENU_COLOR, WINDOW_COLOR, TEXT_COLOR);
	//  COLORS are uint16_t (RGB565 format)
	// .MyMenu.getrgb(byte R, byte G, byte B); - CALCULATING RGB565 format

	//HERCULES MONITOR COLOR SCHEMA
	//MyMenu.setColorSchema(MyMenu.getrgb(163, 51, 165), MyMenu.getrgb(116, 123, 127), MyMenu.getrgb(255, 255, 255));

	// ADD MENU ITEM
	// MyMenu.addMenuItem(SUBMENU_ID,MENU_TITTLE,BTN_A_TITTLE,BTN_B_TITTLE,BTN_C_TITTLE,SELECTOR,FUNCTION_NAME);
	//    SUBMENU_ID byte [0-7]: TOP MENU = 0, SUBMENUs = [1-7]
	//    SELECTOR
	//           IF SELECTOR = -1 then MyMenu.execute() run function with name in last parameter (FUNCTION_NAME)
	//           IF SELECTOR = [0-7] then MyMenu.execute() switch menu items to SUBMENU_ID
	//    FUNCTION_NAME: name of function to run....

	MyMenu.addMenuItem(0, "APPLICATIONS", "<", "OK", ">", 1, "/Data/Apps.jpg", appReturn);
	MyMenu.addMenuItem(0, "SYSTEM", "<", "OK", ">", 2, "/Data/System.jpg", appReturn);
	MyMenu.addMenuItem(0, "ABOUT", "<", "OK", ">", -1, "/Data/About.jpg", appAbout);

	MyMenu.addMenuItem(1, "OSCILOSCOPE", "<", "OK", ">", -1, "/Data/Oscilloscope.jpg", appOsciloscope);
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
	if (WiFi.isConnected())
	{
		unsigned long now = millis();
		if (now - lastcheck >= 1000)
		{
			lastcheck = now;
			M5.Lcd.setTextColor(WHITE, 15);
			SignalStrength = map(100 + WiFi.RSSI(), 5, 90, 0, 100);
			M5.Lcd.drawRightString("WiFi: " + String(SignalStrength) + " %", 310, 5, 2);
		}
		if (!OtaRunning)
		{
			appOta();
			OtaRunning = true;
		}
		ArduinoOTA.handle();
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

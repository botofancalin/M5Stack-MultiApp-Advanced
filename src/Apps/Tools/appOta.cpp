#include "appOta.h"

void appOta()
{
	ArduinoOTA
		.onStart([]() {
			String type;
			if (ArduinoOTA.getCommand() == U_FLASH)
			{
				type = "firmware";
			}
			else // U_SPIFFS
			{
				type = "filesystem";
			}

			// NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
			M5m.windowClr();
			M5m.Lcd.setTextColor(YELLOW);
			M5m.Lcd.drawString(String("Start updating " + type), 5, 60, 2);
		})
		.onEnd([]() {
			M5m.Lcd.drawString("Done", 5, 160, 2);
		})
		.onProgress([](unsigned int progress, unsigned int total) {
			M5m.Lcd.HprogressBar(20, 100, 280, 30, YELLOW, (progress / (total / 100)));
		})
		.onError([](ota_error_t error) {
			M5m.Lcd.println("Error[%u]: " + String(error));
			if (error == OTA_AUTH_ERROR)
				M5m.Lcd.println("Auth Failed");
			else if (error == OTA_BEGIN_ERROR)
				M5m.Lcd.println("Begin Failed");
			else if (error == OTA_CONNECT_ERROR)
				M5m.Lcd.println("Connect Failed");
			else if (error == OTA_RECEIVE_ERROR)
				M5m.Lcd.println("Receive Failed");
			else if (error == OTA_END_ERROR)
				M5m.Lcd.println("End Failed");
		});

	ArduinoOTA.begin();
}
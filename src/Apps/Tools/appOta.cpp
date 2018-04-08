#include "../../apps.h"

void appOta()
{
    	ArduinoOTA
		.onStart([]() {
			String type;
			if (ArduinoOTA.getCommand() == U_FLASH)
			{
				type = "sketch";
			}
			else // U_SPIFFS
			{
				type = "filesystem";
			}

			// NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
            M5.Lcd.setCursor(0,60);
            M5.Lcd.setTextColor(YELLOW);
            M5.Lcd.setTextSize(2);
			M5.Lcd.println("Start updating " + type);
		})
		.onEnd([]() {
			M5.Lcd.println("Done");
		})
		.onProgress([](unsigned int progress, unsigned int total) {
			M5.Lcd.HprogressBar(10, 110, 300, 30, YELLOW, (progress / (total / 100)));
		})
		.onError([](ota_error_t error) {
			M5.Lcd.println("Error[%u]: " + String(error));
			if (error == OTA_AUTH_ERROR)
				M5.Lcd.println("Auth Failed");
			else if (error == OTA_BEGIN_ERROR)
				M5.Lcd.println("Begin Failed");
			else if (error == OTA_CONNECT_ERROR)
				M5.Lcd.println("Connect Failed");
			else if (error == OTA_RECEIVE_ERROR)
				M5.Lcd.println("Receive Failed");
			else if (error == OTA_END_ERROR)
				M5.Lcd.println("End Failed");
		});

	ArduinoOTA.begin();
}
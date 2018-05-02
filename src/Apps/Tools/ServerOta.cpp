#include "ServerOta.h"

void M5StackServerOta::checkForNewVersion()
{
    HTTPClient httpClient;

    httpClient.begin(fwUrlBase); // connect to server vresion file
    int httpCode = httpClient.GET();
    if (httpCode == 200)
    {
        String newFWVersion = httpClient.getString(); //get the server firmware number
        int newVersion = newFWVersion.toInt();

        if (newVersion > version)
        {
            httpClient.end();
            execOTA();
        }
    }
}

// Utility to extract header value from headers
String M5StackServerOta::getHeaderValue(String header, String headerName)
{
    return header.substring(strlen(headerName.c_str()));
}

// OTA Logic
void M5StackServerOta::execOTA()
{
    WiFiClient client;

    if (client.connect(host.c_str(), port))
    {
        // Get the contents of the bin file
        client.print(String("GET ") + bin + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "Cache-Control: no-cache\r\n" +
                     "Connection: close\r\n\r\n");

        unsigned long timeout = millis();
        while (client.available() == 0)
        {
            if (millis() - timeout > 5000)
            {
                client.stop();
                return;
            }
        }

        while (client.available())
        {
            String line = client.readStringUntil('\n'); // read line till /n
            line.trim();                                // remove space, to check if the line is end of headers

            // if the the line is empty,
            // this is end of headers
            // break the while and feed the
            // remaining `client` to the
            // Update.writeStream();
            if (!line.length())
            {
                break; // headers ended, get the OTA started
            }
            // Check if the HTTP Response is 200
            // else break and Exit Update
            if (line.startsWith("HTTP/1.1"))
            {
                if (line.indexOf("200") < 0)
                {
                    break;
                }
            }
            // extract headers here
            // Start with content length
            if (line.startsWith("Content-Length: "))
            {
                contentLength = atoi((getHeaderValue(line, "Content-Length: ")).c_str());
                // Serial.println("Got " + String(contentLength) + " bytes from server");
            }
            // Next, the content type
            if (line.startsWith("Content-Type: "))
            {
                String contentType = getHeaderValue(line, "Content-Type: ");
                if (contentType == "application/octet-stream")
                {
                    isValidContentType = true;
                }
            }
        }
    }
    // check contentLength and content type
    if (contentLength && isValidContentType)
    {
        // Check if there is enough to OTA Update
        bool canBegin = Update.begin(contentLength);

        // If yes, begin
        if (canBegin)
        {
            M5m.Lcd.fillScreen(BLACK);
            M5m.Lcd.setTextColor(YELLOW, BLACK);
            M5m.Lcd.drawCentreString("Please Wait...", 160, 80, 4);
            M5m.Lcd.drawCentreString("Updating Firmware", 160, 120, 4);
            // No activity would appear on the Serial monitor
            // So be patient. This may take 2 - 5mins to complete
            size_t written = Update.writeStream(client);

            if (Update.end())
            {
                if (Update.isFinished())
                {
                    ESP.restart();
                }
            }
        }
        else
        {
            // not enough space to begin OTA
            // Understand the partitions and
            // space availability
            client.flush();
        }
    }
    else
    {
        client.flush();
    }
}

M5StackServerOta::M5StackServerOta(const int *ota_version) : version(*ota_version)
{
}

M5StackServerOta::~M5StackServerOta()
{
}
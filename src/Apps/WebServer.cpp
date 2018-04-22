/*
  SDWebServer - Example WebServer with SD Card backend for esp8266/esp32

  Copyright (c) 2015 Hristo Gochkov. All rights reserved.
  This file is part of the ESP8266/ESP32 WebServer library for Arduino environment.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Have a FAT Formatted SD Card connected to the SPI port of the ESP8266
  The web root is the SD Card root folder
  File extensions with more than 3 charecters are not supported by the SD Library
  File Names longer than 8 charecters will be truncated by the SD library, so keep filenames shorter
  index.htm is the default index (works on subfolders as well)

  upload the contents of SdRoot to the root of the SDcard and access the editor by going to http://esp8266sd.local/edit

*/

#include "../Commons.h"

WebServer server(80);
TaskHandle_t webServerTask = NULL;

File uploadFile;

void returnOK()
{
    server.send(200, "text/plain", "");
}

void returnFail(String msg)
{
    server.send(500, "text/plain", msg + "\r\n");
}

bool loadFromSdCard(String path)
{
    String dataType = "text/plain";
    if (path.endsWith("/"))
    {
        path += "index.htm";
    }

    if (path.endsWith(".src"))
    {
        path = path.substring(0, path.lastIndexOf("."));
    }
    else if (path.endsWith(".htm"))
    {
        dataType = "text/html";
    }
    else if (path.endsWith(".css"))
    {
        dataType = "text/css";
    }
    else if (path.endsWith(".js"))
    {
        dataType = "application/javascript";
    }
    else if (path.endsWith(".png"))
    {
        dataType = "image/png";
    }
    else if (path.endsWith(".gif"))
    {
        dataType = "image/gif";
    }
    else if (path.endsWith(".jpg"))
    {
        dataType = "image/jpeg";
    }
    else if (path.endsWith(".ico"))
    {
        dataType = "image/x-icon";
    }
    else if (path.endsWith(".xml"))
    {
        dataType = "text/xml";
    }
    else if (path.endsWith(".pdf"))
    {
        dataType = "application/pdf";
    }
    else if (path.endsWith(".zip"))
    {
        dataType = "application/zip";
    }

    File dataFile = SD.open(path.c_str());
    if (dataFile.isDirectory())
    {
        path += "/index.htm";
        dataType = "text/html";
        dataFile = SD.open(path.c_str());
    }

    if (!dataFile)
    {
        return false;
    }

    if (server.hasArg("download"))
    {
        dataType = "application/octet-stream";
    }

    if (server.streamFile(dataFile, dataType) != dataFile.size())
    {
        M5.Lcd.println("Sent less data than expected!");
    }

    dataFile.close();
    return true;
}

void handleFileUpload()
{
    if (server.uri() != "/edit")
    {
        return;
    }
    HTTPUpload &upload = server.upload();
    if (upload.status == UPLOAD_FILE_START)
    {
        if (SD.exists((char *)upload.filename.c_str()))
        {
            SD.remove((char *)upload.filename.c_str());
        }
        uploadFile = SD.open(upload.filename.c_str(), FILE_WRITE);
        M5.Lcd.print("Upload: START, filename: ");
        M5.Lcd.println(upload.filename);
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
        if (uploadFile)
        {
            uploadFile.write(upload.buf, upload.currentSize);
        }
        M5.Lcd.print("Upload: WRITE, Bytes: ");
        M5.Lcd.println(upload.currentSize);
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
        if (uploadFile)
        {
            uploadFile.close();
        }
        M5.Lcd.print("Upload: END, Size: ");
        M5.Lcd.println(upload.totalSize);
    }
}

void deleteRecursive(String path)
{
    if (!SD.exists((char *)path.c_str()))
    {
        return;
    }
    File file = SD.open((char *)path.c_str());
    if (!file.isDirectory())
    {
        file.close();
        SD.remove((char *)path.c_str());
        return;
    }

    file.rewindDirectory();
    while (true)
    {
        File entry = file.openNextFile();
        if (!entry)
        {
            break;
        }
        String entryPath = path + "/" + entry.name();
        if (entry.isDirectory())
        {
            entry.close();
            deleteRecursive(entryPath);
        }
        else
        {
            entry.close();
            SD.remove((char *)entryPath.c_str());
        }
        yield();
    }

    SD.rmdir((char *)path.c_str());
    file.close();
}

void handleDelete()
{
    if (server.args() == 0)
    {
        return returnFail("BAD ARGS");
    }
    String path = server.arg(0);
    if (path == "/" || !SD.exists((char *)path.c_str()))
    {
        returnFail("BAD PATH");
        return;
    }
    deleteRecursive(path);
    returnOK();
}

void handleCreate()
{
    if (server.args() == 0)
    {
        return returnFail("BAD ARGS");
    }
    String path = server.arg(0);
    if (path == "/" || SD.exists((char *)path.c_str()))
    {
        returnFail("BAD PATH");
        return;
    }

    if (path.indexOf('.') > 0)
    {
        File file = SD.open((char *)path.c_str(), FILE_WRITE);
        if (file)
        {
            file.write(NULL, 0);
            file.close();
        }
    }
    else
    {
        SD.mkdir((char *)path.c_str());
    }
    returnOK();
}

void printDirectory()
{
    if (!server.hasArg("dir"))
    {
        return returnFail("BAD ARGS");
    }
    String path = server.arg("dir");
    if (path != "/" && !SD.exists((char *)path.c_str()))
    {
        return returnFail("BAD PATH");
    }
    File dir = SD.open((char *)path.c_str());
    path = String();
    if (!dir.isDirectory())
    {
        dir.close();
        return returnFail("NOT DIR");
    }
    dir.rewindDirectory();
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/json", "");
    WiFiClient client = server.client();

    server.sendContent("[");
    for (int cnt = 0; true; ++cnt)
    {
        File entry = dir.openNextFile();
        if (!entry)
        {
            break;
        }

        String output;
        if (cnt > 0)
        {
            output = ",";
        }

        output += "{\"type\":\"";
        if (entry.isDirectory())
        {
            output += "dir";
            output += "\",\"name\":\"";
            output += entry.name() + 1;
        }
        else
        {
            output += "file";
            output += "\",\"name\":\"";
            output += entry.name() + (String(dir.name())).length();
        }

        output += "\"";
        output += "}";

        server.sendContent(output);
        entry.close();
    }
    server.sendContent("]");
    server.sendContent("");
    dir.close();
}

void handleNotFound()
{
    if (loadFromSdCard(server.uri()))
    {
        return;
    }
    String message = "SDCARD Not Detected\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (int i = 0; i < server.args(); i++)
    {
        message += " NAME:" + server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
}

void MywebServer(void *parameter)
{
    if (WiFi.localIP().toString() != "0.0.0.0" || WiFi.getMode() == 3)
    {

            M5.Lcd.drawString("HTTP server started", 10, 40, 4);
            if (WiFi.getMode() == 3)
            {
                M5.Lcd.drawString("http://192.168.4.1", 10, 70, 4);
            }
            else
            {
                M5.Lcd.drawString("http://" + WiFi.localIP().toString(), 10, 70, 4);
            }

        server.on("/list", HTTP_GET, printDirectory);
        server.on("/edit", HTTP_DELETE, handleDelete);
        server.on("/edit", HTTP_PUT, handleCreate);
        server.on("/edit", HTTP_POST, []() { returnOK(); }, handleFileUpload);
        server.onNotFound(handleNotFound);

        server.begin();
        M5.Lcd.drawString("If you exit this app,", 10, 130, 2);
        M5.Lcd.drawString("the server will continue to run", 10, 150, 2);
        M5.Lcd.drawString("untill you stop it.", 10, 170, 2);
    }
    for (;;)
    {
        server.handleClient();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void appWebServer()
{
    MyMenu.drawAppMenu(F("WebServer"), F("START"), F("ESC"), F("STOP"));

    while (M5.BtnB.wasPressed())
    {
        M5.update();
    }

    while (!M5.BtnB.wasPressed())
    {
        M5.update();
        if (M5.BtnC.wasPressed())
        {
            MyMenu.windowClr();
            if (webServerTask != NULL)
            {
                vTaskDelete(webServerTask);
                webServerTask = NULL;
                M5.Lcd.drawString("MDNS server stoped", 10, 40, 4);
                M5.Lcd.drawString("HTTP server stoped", 10, 70, 4);
            }
        }

        if (M5.BtnA.wasPressed())
        {
            if (webServerTask == NULL)
            {
                MyMenu.windowClr();
                xTaskCreatePinnedToCore(
                    MywebServer,    /* Task function. */
                    "MywebServer",  /* name of the task, a name just for humans */
                    8192,           /* Stack size of task */
                    NULL,           /* parameter of the task */
                    1,              /* priority of the task */
                    &webServerTask, /* Task handle to keep track of the created task */
                    0);             /*cpu core number where the task is assigned*/
            }
        }
    }
    MyMenu.show();
}

#include "MyWebServer.h"

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
    else if (path.endsWith(".mp3"))
    {
        dataType = "audio/mpeg";
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

    File dataFile = My_SD.open(path.c_str());
    if (dataFile.isDirectory())
    {
        path += "/index.htm";
        dataType = "text/html";
        dataFile = My_SD.open(path.c_str());
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
        Serial.println("Sent less data than expected!");
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
        if (My_SD.exists((char *)upload.filename.c_str()))
        {
            My_SD.remove((char *)upload.filename.c_str());
        }
        uploadFile = My_SD.open(upload.filename.c_str(), FILE_WRITE);
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
        if (uploadFile)
        {
            uploadFile.write(upload.buf, upload.currentSize);
        }
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
        if (uploadFile)
        {
            uploadFile.close();
        }
    }
}

void deleteRecursive(String path)
{
    if (!My_SD.exists((char *)path.c_str()))
    {
        return;
    }
    File file = My_SD.open((char *)path.c_str());
    if (!file.isDirectory())
    {
        file.close();
        My_SD.remove((char *)path.c_str());
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
            My_SD.remove((char *)entryPath.c_str());
        }
        yield();
    }

    My_SD.rmdir((char *)path.c_str());
    file.close();
}

void handleDelete()
{
    if (server.args() == 0)
    {
        return returnFail("BAD ARGS");
    }
    String path = server.arg(0);
    if (path == "/" || !My_SD.exists((char *)path.c_str()))
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
    if (path == "/" || My_SD.exists((char *)path.c_str()))
    {
        returnFail("BAD PATH");
        return;
    }

    if (path.indexOf('.') > 0)
    {
        File file = My_SD.open((char *)path.c_str(), FILE_WRITE);
        if (file)
        {
            file.write(0);
            file.close();
        }
    }
    else
    {
        My_SD.mkdir((char *)path.c_str());
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
    if (path != "/" && !My_SD.exists((char *)path.c_str()))
    {
        return returnFail("BAD PATH");
    }
    File dir = My_SD.open((char *)path.c_str());
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

        M5m.Lcd.drawString("HTTP server started", 10, 40, 4);
        if (WiFi.getMode() == 3)
        {
            M5m.Lcd.drawString("http://192.168.4.1", 10, 70, 4);
        }
        else
        {
            M5m.Lcd.drawString("http://" + WiFi.localIP().toString(), 10, 70, 4);
        }

        server.on("/list", HTTP_GET, printDirectory);
        server.on("/edit", HTTP_DELETE, handleDelete);
        server.on("/edit", HTTP_PUT, handleCreate);
        server.on("/edit", HTTP_POST, []() { returnOK(); }, handleFileUpload);
        server.onNotFound(handleNotFound);

        server.begin();
        M5m.Lcd.drawString("If you exit this app,", 10, 130, 2);
        M5m.Lcd.drawString("the server will continue to run", 10, 150, 2);
        M5m.Lcd.drawString("untill you stop it.", 10, 170, 2);
    }
    for (;;)
    {
        server.handleClient();
        //vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void WebServerClass::Run()
{
    while (!M5m.BtnB.wasPressed())
    {
        M5m.update();
        if (M5m.BtnC.wasPressed())
        {
            M5m.windowClr();
            if (webServerTask != NULL)
            {
                vTaskDelete(webServerTask);
                webServerTask = NULL;
                M5m.Lcd.drawString("MDNS server stoped", 10, 40, 4);
                M5m.Lcd.drawString("HTTP server stoped", 10, 70, 4);
            }
        }

        if (M5m.BtnA.wasPressed())
        {
            if (webServerTask == NULL)
            {
                M5m.windowClr();
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
}

WebServerClass::WebServerClass()
{
    M5m.update();
    M5m.drawAppMenu(F("WebServer"), F("START"), F("ESC"), F("STOP"));
}

WebServerClass::~WebServerClass()
{
    M5m.show();
}
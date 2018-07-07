#include "SdBrowser.h"

void SdBrowserClass::listDir(fs::FS &fs, String dirName, int levels)
{
    File root = fs.open(dirName);
    if (!root)
    {
        return;
    }
    if (!root.isDirectory())
    {
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (!strstr((file.name()), ignored))
        {
            if (file.isDirectory())
            {
                if (levels)
                {
                    listDir(fs, file.name(), levels - 1);
                }
            }
            else
            {
                fileVector.push_back(fileinfo);
                String fileName = file.name();
                fileVector[appsCount].fileName = fileName;
                fileVector[appsCount].fileSize = file.size();
                appsCount++;
            }
        }
        file = root.openNextFile();
    }
    file.close();
}

void SdBrowserClass::aSortFiles()
{
    bool swapped;
    String name1, name2;
    do
    {
        swapped = false;
        for (int i = 0; i < appsCount - 1; i++)
        {
            name1 = String(fileVector[i].fileName[0]);
            name2 = String(fileVector[i + 1].fileName[0]);
            if (name1 == name2)
            {
                name1 = String(fileVector[i].fileName[1]);
                name2 = String(fileVector[i + 1].fileName[1]);
                if (name1 == name2)
                {
                    name1 = String(fileVector[i].fileName[2]);
                    name2 = String(fileVector[i + 1].fileName[2]);
                }
            }

            if (name1 > name2)
            {
                fileinfo = fileVector[i];
                fileVector[i] = fileVector[i + 1];
                fileVector[i + 1] = fileinfo;
                swapped = true;
            }
        }
    } while (swapped);
}

void SdBrowserClass::buildMyMenu()
{
    M5m.clearList();
    M5m.setListCaption("Files");
    for (int i = 0; i < appsCount; i++)
    {
        String shortName = fileVector[i].fileName.substring(1);
        M5m.addList(shortName);
    }
}

void SdBrowserClass::Run()
{
    appsCount = 0;
    listDir(SD, "/", levels);
    aSortFiles();
    buildMyMenu();
    M5m.showList();
    while (!M5m.BtnA.wasPressed())
    {
        if (M5m.BtnC.wasPressed())
        {
            if (repaint)
            {
                M5m.Lcd.clear();
                M5m.drawAppMenu(F("SD BROWSER"), F("EXIT"), F("OPEN"), F(">"));
                M5m.btnRestore();
            }
            repaint = false;
            M5m.nextList();
        }
        int MenuID = M5m.getListID();

        if (M5m.BtnB.wasPressed())
        {
            String FileName = fileVector[MenuID].fileName;
            if (FileName.endsWith(".jpg"))
            {
                M5m.Lcd.clear();
                if (fileVector[MenuID].fileSize > 100000UL)
                {
                    M5m.Lcd.drawJpgFile(SD, FileName.c_str(), 0, 0, 0, 0, 0, 0, JPEG_DIV_8);
                }
                else if (fileVector[MenuID].fileSize > 70000UL)
                {
                    M5m.Lcd.drawJpgFile(SD, FileName.c_str(), 0, 0, 0, 0, 0, 0, JPEG_DIV_4);
                }
                else if (fileVector[MenuID].fileSize > 50000)
                {
                    M5m.Lcd.drawJpgFile(SD, FileName.c_str(), 0, 0, 0, 0, 0, 0, JPEG_DIV_2);
                }
                else
                {
                    M5m.Lcd.drawJpgFile(SD, FileName.c_str());
                }
                repaint = true;
                M5m.btnRestore();
            }
            else if (FileName.endsWith(".mp3"))
            {
                Mp3PlayerClass Mp3PlayerObj;
                Mp3PlayerObj.Play(&FileName);
            }
            else if (FileName.endsWith(".mov"))
            {
                VideoPlayerClass VideoPlayerObj;
                VideoPlayerObj.Play(FileName.c_str());
            }
            else if (!inmenu)
            {
                inmenu = true;
                M5m.windowClr();
                M5m.Lcd.drawCentreString("File Name: " + fileVector[MenuID].fileName, M5m.Lcd.width() / 2, (M5m.Lcd.height() / 2) - 10, 2);
                M5m.Lcd.drawCentreString("File Size: " + String(fileVector[MenuID].fileSize), M5m.Lcd.width() / 2, (M5m.Lcd.height() / 2) + 10, 2);
            }
            else
            {
                inmenu = false;
                M5m.showList();
            }
        }
        M5m.update();
    }
}

SdBrowserClass::SdBrowserClass()
{
    M5m.update();
    M5m.drawAppMenu(F("SD BROWSER"), F("EXIT"), F("OPEN"), F(">"));
}

SdBrowserClass::~SdBrowserClass()
{
    fileVector.clear();
    fileVector.shrink_to_fit();
    M5m.clearList();
    M5m.show();
}
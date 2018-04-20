#include "../apps.h"

const char *ignored = "System Volume Information";
bool repaint = false;
bool inmenu = false;
int appsCount = 0;
int levels = 1;

struct FileInfo
{
    String fileName; // heap
    int fileSize;
};
FileInfo fileinfo;
std::vector<FileInfo> fileVector;

void listDir(fs::FS &fs, String dirName, int levels)
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
}

void aSortFiles()
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

void buildMyMenu()
{
    MyMenu.clearList();
    MyMenu.setListCaption("Files");
    for (int i = 0; i < appsCount; i++)
    {
        String shortName = fileVector[i].fileName.substring(1);
        MyMenu.addList(shortName);
    }
}

void appBrowser()
{
    appsCount = 0;
    M5.update();
    MyMenu.drawAppMenu(F("SD BROWSER"), F("EXIT"), F("OPEN"), F(">"));
    listDir(SD, "/", levels);
    aSortFiles();
    buildMyMenu();
    MyMenu.showList();
    while (!M5.BtnA.wasPressed())
    {
        if (M5.BtnC.wasPressed())
        {
            if (repaint)
            {
                M5.Lcd.clear();
                MyMenu.drawAppMenu(F("SD BROWSER"), F("EXIT"), F("OPEN"), F(">"));
                MyMenu.btnRestore();
            }
            MyMenu.nextList();
        }
        int MenuID = MyMenu.getListID();

        if (M5.BtnB.wasPressed())
        {
            String FileName = fileVector[MenuID].fileName;
            if (FileName.endsWith(".jpg"))
            {
                M5.Lcd.clear();
                if (fileVector[MenuID].fileSize > 100000UL)
                {
                    M5.Lcd.drawJpgFile(SD, FileName.c_str(), 0, 0, 0, 0, 0, 0, JPEG_DIV_8);
                }
                else if (fileVector[MenuID].fileSize > 70000UL)
                {
                    M5.Lcd.drawJpgFile(SD, FileName.c_str(), 0, 0, 0, 0, 0, 0, JPEG_DIV_4);
                }
                else if (fileVector[MenuID].fileSize > 50000)
                {
                    M5.Lcd.drawJpgFile(SD, FileName.c_str(), 0, 0, 0, 0, 0, 0, JPEG_DIV_2);
                }
                else
                {
                    M5.Lcd.drawJpgFile(SD, FileName.c_str());
                }
                repaint = true;
                MyMenu.btnRestore();
            }
            else if (FileName.endsWith(".mp3"))
            {
                mp3Player(&FileName);
            }
            else if (!inmenu)
            {
                inmenu = true;
                MyMenu.windowClr();
                M5.Lcd.drawCentreString("File Name: " + fileVector[MenuID].fileName, M5.Lcd.width() / 2, (M5.Lcd.height() / 2) - 10, 2);
                M5.Lcd.drawCentreString("File Size: " + String(fileVector[MenuID].fileSize), M5.Lcd.width() / 2, (M5.Lcd.height() / 2) + 10, 2);
            }
            else
            {
                inmenu = false;
                MyMenu.showList();
            }
        }
        M5.update();
    }
    fileVector.clear();
    fileVector.shrink_to_fit();
    MyMenu.clearList();
    MyMenu.show();
}
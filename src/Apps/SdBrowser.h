#include "M5StackSAM.h"
#include "Tools/Mp3Player.h"

class SdBrowserClass
{
  public:
    SdBrowserClass();
    ~SdBrowserClass();

    void Run();

  private:
    const char * const ignored = "System Volume Information";
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

    void listDir(fs::FS &fs, String dirName, int levels);
    void aSortFiles();
    void buildMyMenu();
};
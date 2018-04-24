#include "M5StackSAM.h"
#include "AudioFileSourceICYStream.h"
#include "AudioFileSourceBuffer.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2SNoDAC.h"

class WebRadioClass
{
  public:
    WebRadioClass();
    ~WebRadioClass();
    void Run();

  private:
    void getvolume();
    void setVolume(int *v);
   // void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string);
   // void StatusCallback(void *cbData, int code, const char *string);

    
    const char *URL = "http://astreaming.europafm.ro:8000/europafm_mp3_64k";
    
    const int preallocateBufferSize = 16 * 1024;
    //const int preallocateCodecSize = 16384; // AAC+SBR codec max mem needed
    void *preallocateBuffer = NULL;
    //void *preallocateCodec = NULL;
    bool play = true;
    bool upd = true;

    AudioGeneratorMP3 *mp3;
    AudioFileSourceICYStream *file;
    AudioFileSourceBuffer *buff;
    AudioOutputI2S *out;
    AudioStatus *stat;
};

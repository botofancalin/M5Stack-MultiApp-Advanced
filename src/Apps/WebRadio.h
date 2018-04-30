#include "M5StackSAM.h"
#include "AudioFileSourceICYStream.h"
#include "AudioFileSourceBuffer.h"
#include "AudioGeneratorMP3a.h"
#include "AudioGeneratorMP3.h"
#include "AudioGeneratorAAC.h"
#include "AudioOutputI2S.h"
class WebRadioClass
{
public:
  WebRadioClass();
  ~WebRadioClass();

  void Run();

private:
  AudioGenerator *decoder = NULL;
  AudioFileSourceICYStream *file = NULL;
  AudioFileSourceBuffer *buff = NULL;
  AudioOutputI2S *out = NULL;

  const int preallocateBufferSize = 5 * 1024;
  const int preallocateCodecSize = 29192; // AAC+SBR codec max mem needed
  void *preallocateBuffer = NULL;
  void *preallocateCodec = NULL;
  bool isAAC = false;

  std::vector<String> Name;
  std::vector<String> Link;
  String StationList, name, link, old_Station = "";
  int SignalStrength;
  unsigned int Station = 0;
  unsigned long now;
  unsigned long lastcheck;
  bool play = true;
  bool upd = true;

  void getvolume();
  void setVolume(int *v);
  bool GetStations(fs::FS &fs, const char *path);
  void StopPlaying();
};

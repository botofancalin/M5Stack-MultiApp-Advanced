#pragma once
#include "M5StackMod.h"
#define MDFONT &FreeSerif12pt7b
#define STFONT &FreeSerif18pt7b

class WebRadioClass
{
public:
  WebRadioClass();
  ~WebRadioClass();

  void Run();

private:
  AudioGenerator *player = NULL;
  AudioFileSourceICYStream *file = NULL;
  AudioFileSourceBuffer *buff = NULL;
  AudioOutputI2S *out = NULL;

  const int preallocateBufferSize = 16384;
  const int preallocateCodecSize = 29192;
  void *preallocateBuffer = NULL;
  void *preallocateCodec = NULL;

  std::vector<String> Name;
  std::vector<String> Link;
  String StationList, name, link, old_Station = "";
  int SignalStrength;
  unsigned int Station = 0;
  unsigned int fillLvl, rawFillLvl = 1;
  unsigned long now;
  unsigned long lastcheck;
  bool play = true;
  bool upd = true;

  void getvolume();
  void setVolume(int *v);
  bool GetStations(fs::FS &fs, const char *path);
  void StopPlaying();
};

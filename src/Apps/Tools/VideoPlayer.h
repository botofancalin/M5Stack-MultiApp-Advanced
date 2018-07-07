#pragma once
#include "M5StackMod.h"

class VideoPlayerClass
{
  private:
    uint8_t *videoBuffer = NULL;
    File file;

  public:
  void Play(const char *fileName);
    VideoPlayerClass();
    ~VideoPlayerClass();
};



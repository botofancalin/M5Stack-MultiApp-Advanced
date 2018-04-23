#include "M5StackSAM.h"

class StopWatchClass
{
  public:
    StopWatchClass();
    ~StopWatchClass();

    void Run();

  private:
    bool tmp_run = false;
    float tmp_sec = 0;
    unsigned int tmp_min = 0;
    unsigned long tmp_tmr = 0;
};

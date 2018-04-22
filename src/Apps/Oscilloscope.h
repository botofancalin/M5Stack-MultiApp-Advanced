#ifndef Oscilloscope_h
#define Oscilloscope_h

#include "M5StackSAM.h"
#define CH1COLOR YELLOW
#define CH2COLOR MAGENTA
#define OSCGREY 0x7BEF
#define LCD_WIDTH 320
#define LCD_HEIGHT 240
#define SAMPLES LCD_WIDTH
#define RANGE_MIN 0
#define RANGE_MAX 4
#define DOTS_DIV 30
#define MODE_ON 0
#define MODE_INV 1
#define MODE_OFF 2
#define TRIG_AUTO 0
#define TRIG_NORM 1
#define TRIG_SCAN 2
#define TRIG_E_UP 0
#define TRIG_E_DN 1
#define RATE_MIN 0
#define RATE_MAX 12

class OscilloscopeClass
{
public:
  OscilloscopeClass();
  ~OscilloscopeClass();

  void Run();

private:
  TaskHandle_t LedC_Gen = NULL;
  TaskHandle_t Syn_Gen = NULL;
  const uint8_t ad_ch0 = 35;
  const uint8_t ad_ch1 = 36;
  int range0 = RANGE_MIN;
  int range1 = RANGE_MIN;
  uint8_t ch0_mode = MODE_ON;
  uint8_t ch1_mode = MODE_OFF;
  int trig_mode = TRIG_AUTO;
  int trig_edge = TRIG_E_DN;
  int trig_ch = 0;
  int menu = 19;
  int rate = 2;
  int trig_lv = 40;
  int ch0_off = 0;
  int ch1_off = 0;
  unsigned int data[4][SAMPLES]; // keep twice of the number of channels to make it a double buffer
  const int VREF[5] PROGMEM = {250, 500, 1250, 2500, 5000};
  const int MILLIVOL_per_dot[5] PROGMEM = {33, 17, 6, 3, 2};
  const char *Modes[3] PROGMEM = {"NORM", "INV", "OFF"};
  const char *TRIG_Modes[3] PROGMEM = {"Auto", "Norm", "Scan"};
  const char *Rates[13] PROGMEM = {"  F1", "  F2", " 5ms", "10ms", "20ms", "50ms", "0.1s", "0.2s", "0.5s", "1s", "2s", "5s", "10s"};
  const char *Ranges[5] PROGMEM = {" 1V", "0.5V", "0.2V", "0.1V", "50mV"};
  unsigned int sample = 0; // index for double buffer
  bool Start = true;
  bool exitprg = false;

  void DrawText();
  void CheckSW();
  void DrawGrid();
  void DrawGrid(int x);
  void ClearAndDrawGraph();
  void ClearAndDrawDot(int i);
  inline unsigned int adRead(const uint8_t *ch, uint8_t *mode, int *off);
};

#endif

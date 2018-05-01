#pragma once
#include "M5Stack.h"

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
#define RATE_MAX 14
class OscilloscopeClass
{
public:
  OscilloscopeClass();
  ~OscilloscopeClass();

  void Run();

private:
  TaskHandle_t Sig_Gen = NULL;
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
  int rate = 3;
  unsigned int r, ad, oad, trig_lv = 40;
  unsigned int * a = &r;
  unsigned int * adp = &ad;
  unsigned int * oadp = &oad;
  int16_t ch0_off = 0;
  int16_t ch1_off = 0;
  unsigned int data[4][SAMPLES]; // keep twice of the number of channels to make it a double buffer
  const int VREF[5] PROGMEM = {250, 500, 1250, 2500, 5000};
  const int MILLIVOL_per_dot[5] PROGMEM = {33, 17, 6, 3, 2};
  const char *Modes[3] PROGMEM = {"NORM", "INV", "OFF"};
  const char *TRIG_Modes[3] PROGMEM = {"Auto", "Norm", "Scan"};
  const char *Rates[15] PROGMEM = {"0.3ms", "0.6ms", " 1ms", " 2ms", " 5ms", "10ms", "20ms", "50ms", "0.1s", "0.2s", "0.5s", "1s", "2s", "5s", "10s"};
  const char *Ranges[5] PROGMEM = {" 1V", "0.5V", "0.2V", "0.1V", "50mV"};
  const char *Generator[5] PROGMEM = {"Wave", "Sqr", "Tri1", "Tri2", "OFF"};
  unsigned int sample = 0; // index for double buffer
  bool Start = true;
  bool exitprg = false;

  void DrawText();
  void CheckSW();
  void DrawGrid();
  void DrawGrid(int x);
  void ClearAndDrawGraph();
  void ClearAndDrawDot(int i);
  inline unsigned int adRead(const uint8_t *ch, uint8_t *mode, int16_t *off);
};

inline unsigned int OscilloscopeClass::adRead(const uint8_t *ch, uint8_t *mode, int16_t *off)
{
	*a = analogRead(*ch);
	*a = (((*a + *off) * VREF[(*ch == ad_ch0) ? range0 : range1]) / 10000) + 30;
	*a = ((*a >= LCD_HEIGHT) ? LCD_HEIGHT : *a);
	if (*mode == MODE_INV)
	{
		return LCD_HEIGHT - *a;
	}
	return *a;
}
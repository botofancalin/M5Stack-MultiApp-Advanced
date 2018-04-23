#include "M5StackSAM.h"
#include "esp_wifi.h"

#define MAX_CH 14
#define SNAP_LEN 2324 // max len of each recieved packet
#define MAX_X 315	 // 128
#define MAX_Y 230	 //  51

class WifiPacketMonitorClass
{
public:
    WifiPacketMonitorClass();
    ~WifiPacketMonitorClass();

void Run();

private:
unsigned long lastDrawTime;
unsigned long currentTime;
uint32_t pkts[MAX_X]; // here the packets per second will be saved
uint8_t ch = 1;		  // current 802.11 channel
int s = 10, a = 0;
int len, rssi;

esp_err_t event_handler(void *ctx, system_event_t *event);
double getMultiplicator();
void setChannel(int newChannel);
void draw();
};
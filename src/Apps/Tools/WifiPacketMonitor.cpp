#include "../../apps.h"

#define MAX_CH 14 
#define SNAP_LEN 2324 // max len of each recieved packet
#define MAX_X 315     // 128
#define MAX_Y 230     //  51

uint32_t lastDrawTime;
uint32_t tmpPacketCounter;
uint32_t pkts[MAX_X]; // here the packets per second will be saved
uint32_t deauths = 0; // deauth frames per second
unsigned int ch = 1;  // current 802.11 channel
int rssiSum;

esp_err_t event_handler(void *ctx, system_event_t *event)
{
	return ESP_OK;
}

double getMultiplicator()
{
	uint32_t maxVal = 1;
	for (int i = 0; i < MAX_X; i++)
	{
		if (pkts[i] > maxVal)
		{
			maxVal = pkts[i];
		}
	}
	if (maxVal > MAX_Y)
	{
		return (double)MAX_Y / (double)maxVal;
	}
	else
	{
		return 1;
	}
}

void wifi_promiscuous(void *buf, wifi_promiscuous_pkt_type_t type)
{
	wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
	wifi_pkt_rx_ctrl_t ctrl = (wifi_pkt_rx_ctrl_t)pkt->rx_ctrl;
	if (type == WIFI_PKT_MGMT && (pkt->payload[0] == 0xA0 || pkt->payload[0] == 0xC0))
	{
		deauths++;
	}
	if (type == WIFI_PKT_MISC)
	{
		return; // wrong packet type
	}
	if (ctrl.sig_len > SNAP_LEN)
	{
		return; // packet too long
	}
	uint32_t packetLength = ctrl.sig_len;
	if (type == WIFI_PKT_MGMT)
	{
		packetLength -= 4;
	}
	tmpPacketCounter++;
	rssiSum += ctrl.rssi;
}

void setChannel(int newChannel)
{
	ch = newChannel;
	if (ch > MAX_CH)
	{
		ch = 1;
	}
	else if (ch < 1)
	{
		ch = 14;
	}
	esp_wifi_set_promiscuous(false);
	esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);
	esp_wifi_set_promiscuous_rx_cb(&wifi_promiscuous);
	esp_wifi_set_promiscuous(true);
}

void draw()
{
	double multiplicator = getMultiplicator();
	int len, rssi;
	if (pkts[MAX_X - 1] > 0)
	{
		rssi = rssiSum / (int)pkts[MAX_X - 1];
	}
	else
	{
		rssi = rssiSum;
	}
	String p = "Ch: " + (String)ch + " | Rssi: " + (String)rssi + " | Packets: " +
		(String)tmpPacketCounter + " | Deauth: [" + deauths + "]";
	M5.Lcd.setTextColor(WHITE, BLUE);
	M5.Lcd.drawString(p + "  ", 10, 2, 2);                          // string DRAW
	M5.Lcd.drawLine(40, MAX_Y - 200, MAX_X, MAX_Y - 200, GREEN); // MAX LINE DRAW
	for (int i = 40; i < MAX_X; i++)
	{
		len = pkts[i] * multiplicator;
		len = len * 2;
		if ((MAX_Y - len) < (MAX_Y - 200))
		{
			len = 200;
		}
		M5.Lcd.drawLine(i, MAX_Y, i, 31, BLACK);      // LINE EARSE
		M5.Lcd.drawLine(i, MAX_Y, i, MAX_Y - len, GREEN); // LINE DRAW
		if (i < MAX_X - 1)
		{
			pkts[i] = pkts[i + 1];
		}
		M5.Lcd.setTextColor(RED);
		M5.Lcd.drawString("Ch-", 57, 210, 2);
		M5.Lcd.drawString("Exit", 148, 210, 2);
		M5.Lcd.drawString("Ch+", 247, 210, 2);
	}
}
// ===== main program ================================================
void Monitor_run()
{
	WiFi.disconnect();
	WiFi.mode(WIFI_OFF);
	esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);
	M5.Lcd.fillScreen(BLACK);
	M5.Lcd.setTextColor(WHITE, BLACK);;
	int s = 10, a = 0;
	for (int ypos = MAX_Y; ypos > 120; ypos = ypos - s)
	{
		M5.Lcd.setTextDatum(MR_DATUM);
		M5.Lcd.drawString(String(MAX_Y - ypos), 30, ypos - 1 - a, 2);
		a = a + 10;
	}
	M5.Lcd.setTextDatum(TL_DATUM);
	M5.Lcd.fillRect(0, 0, 320, 20, BLUE);
	esp_wifi_set_promiscuous_rx_cb(&wifi_promiscuous);
	esp_wifi_set_promiscuous(true);
	uint32_t currentTime;
	while (1)
	{
		currentTime = millis();
		{
			M5.update();
			if (M5.BtnA.wasPressed())
			{
				ch--;
				setChannel(ch);
				draw();
			}
			if (M5.BtnC.wasPressed())
			{
				ch++;
				setChannel(ch);
				draw();
			}
			if (M5.BtnB.wasPressed())
			{
				break;
			}
		}
		if (currentTime - lastDrawTime > 2000)
		{
			lastDrawTime = currentTime;
			pkts[MAX_X - 1] = tmpPacketCounter;
			draw();
			tmpPacketCounter = 0;
			deauths = 0;
			rssiSum = 0;
		}
	}
	WiFi.disconnect();
	M5.Lcd.setTextFont(1);
	return;
}
#ifndef _CONFIG_H_
#define _CONFIG_H_

//#define M5STACK

#define SPI_FREQUENCY  40000000

// For the M5Stack module use these #define lines
#ifdef M5STACK

#define My_SD SD
// Screen
#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   14  // Chip select control pin
#define TFT_DC   27  // Data Command control pin
#define TFT_RST  33  // Reset pin (could connect to Arduino RESET pin)
#define TFT_BL   32  // LED back-light

// SD card
#define TFCARD_CS_PIN 4

// Buttons
#define BTN_A 0
#define BTN_B 1
#define BTN_C 2
#define BUTTON_A 0
#define BUTTON_B 1
#define BUTTON_C 2
#define BUTTON_A_PIN 39
#define BUTTON_B_PIN 38
#define BUTTON_C_PIN 37

#define SDA_PIN 21
#define SCL_PIN 22

// LORA
#define LORA_CS_PIN   5
#define LORA_RST_PIN  26
#define LORA_IRQ_PIN  36

#else

#define My_SD SD_MMC

#define TFT_MISO 25
#define TFT_MOSI 23
#define TFT_SCLK 19
#define TFT_CS   22  // Chip select control pin
#define TFT_DC   21  // Data Command control pin
#define TFT_RST  18  // Reset pin (could connect to Arduino RESET pin)
#define TFT_BL   5  // LED back-light
// SD card
//#define TFCARD_CS_PIN 14
#define SDA_PIN 36
#define SCL_PIN 34

// Buttons
#define BTN_A 0
#define BTN_B 1
#define BTN_C 2
#define BUTTON_A 0
#define BUTTON_B 1
#define BUTTON_C 2
#define BUTTON_A_PIN 32
#define BUTTON_B_PIN 27
#define BUTTON_C_PIN 33

#endif
// UART
#define USE_SERIAL Serial

#endif /* SETTINGS_C */

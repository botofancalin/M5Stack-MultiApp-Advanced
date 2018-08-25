#ifndef _CONFIG_H_
#define _CONFIG_H_

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
#define BUTTON_A_PIN 39
#define BUTTON_B_PIN 38
#define BUTTON_C_PIN 37

#define SDA_PIN 21
#define SCL_PIN 22

#elif WROVER_KIT

#define SPI_FREQUENCY  40000000

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
#define BUTTON_A_PIN 32
#define BUTTON_B_PIN 27
#define BUTTON_C_PIN 33

#else 

#define SPI_FREQUENCY  27000000

#define My_SD SD

#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   5  // Chip select control pin
#define TFT_DC   17  // Data Command control pin
#define TFT_RST  -1  // Reset pin (could connect to Arduino RESET pin)
#define TFT_BL   16  // LED back-light
// SD card
//#define TFCARD_CS_PIN 14
#define SDA_PIN 21
#define SCL_PIN 22

// Buttons
#define BUTTON_A_PIN 32
#define BUTTON_B_PIN 33
#define BUTTON_C_PIN 34

#endif
// UART
#define USE_SERIAL Serial

#endif /* SETTINGS_C */

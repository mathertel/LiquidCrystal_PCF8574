#define __BOARD__ESP8266__

#define APP_VERSION   "BetaConsole V1.2"
#define BUILDTIME __TIME__ " " __DATE__    // utilisé pour créé un UID

#define LED_ON LOW



//Pin out NODEMCU ESP8266
#define D0  16    //!LED_LIFE
#define D1  5     //       I2C_SDA
#define D2  4     //       I2C_SCL
#define D3  0     //!BP0    Alim PN532 (out)
#define D4  2     //!LED2  RF24_CE  + POWER SUPLY I2C PN532

#define D5  14    //!SPI_CLK
#define D6  12    //!SPI_MISO 
#define D7  13    //!SPI_MOSI 
#define D8  15    //        RF24_CS

// GPIO Externe
//#define GP0     //Pousoir Action (in)
//#define GP7     //

#define LED_LIFE D0
#define I2C_SDA  D1
#define I2C_SCL  D2
#define BP0      D3   
#define POWER_PN532  D3
//#define LED_2    D4
#define RF24_CE  D4
//!SPI_CLK       D5
//!SPI_MISO      D6
//!SPI_MOSO      D7
#define RF24_CS  D8

#define BP_ACTION    GP0

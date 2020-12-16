// LCD with i2c backpack test
// This replaces the contents of an LCD screen with the contents of a buffer
// Urs Utzinger, 2020

#include "LiquidCrystal_PCF8574.h"
#include <Wire.h>

TwoWire Wire_1 = TwoWire();

LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 
char lcdDisplay[4][20];           // 4 lines of 20 character buffer

void setup()
{
  Wire_1.begin(D1, D2);  // custom i2c port on ESP
  Wire_1.setClock(100000); // standard 100kHz speed
  Wire_1.setClockStretchLimit(200000); // some devices might need clock stretching
  lcd.begin(20,4, Wire_1);
  lcd.setBacklight(255);
}

void loop()
{
  char lcdbuf[21];
  const char someLine[] = "0123456789ABCDEF1234";  // 20 chars
  
  // fill the screen buffer
  strncpy(&lcdDisplay[0][0], someLine , 20);
  strncpy(&lcdDisplay[1][0], someLine , 20);
  strncpy(&lcdDisplay[2][0], someLine , 20);
  strncpy(&lcdDisplay[3][0], someLine , 20);
  // prepare screen
  lcd.home();
  lcd.clear();
  lcd.setCursor(0, 0);   
  // copy the buffer to the screen
  // 1st line continues at 3d line
  // 2nd line continues at 4th line
  strncpy(lcdbuf, &lcdDisplay[0][0], 20); lcdbuf[20] = '\0'; // create a termineted text line 
  lcd.print(lcdbuf);                                         // print the line to screen
  strncpy(lcdbuf, &lcdDisplay[2][0], 20); lcdbuf[20] = '\0';
  lcd.print(lcdbuf); 
  strncpy(lcdbuf, &lcdDisplay[1][0], 20); lcdbuf[20] = '\0';
  lcd.print(lcdbuf); 
  strncpy(lcdbuf, &lcdDisplay[3][0], 20); lcdbuf[20] = '\0';
  lcd.print(lcdbuf);

  delay(50); // keep artitically short to test if screen corrupts with frequent writes
}

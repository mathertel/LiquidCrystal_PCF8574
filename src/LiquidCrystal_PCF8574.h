/// \file LiquidCrystal_PCF8574.h
/// \brief LiquidCrystal library with PCF8574 I2C adapter.
///
/// \author Matthias Hertel, http://www.mathertel.de
///
/// \copyright Copyright (c) 2019 by Matthias Hertel.\n
///
/// The library work is licensed under a BSD style license.\n
/// See http://www.mathertel.de/License.aspx
///
/// \details
/// This library is can drive a Liquid Cristal display based on the Hitachi HD44780 chip that is connected through a PCF8574 I2C adapter.
/// This is given by many (not all) LCD adapters. This library uses the original Wire library for communication.
/// The API if common to many LCD libraries and documented in https://www.arduino.cc/en/Reference/LiquidCrystal.
/// and partially functions from https://playground.arduino.cc/Code/LCDAPI/.

///
/// ChangeLog:
/// --------
/// * 19.10.2013 created.
/// * 05.06.2019 rewrite from scratch.
/// add by NET234
/// 07/03/2020   optimise use of I2C to limit use of wire.beginTransmission 
/// 02/07/2020   optimise clear to full reinit LCD (allow cuting power and re initialise with a clear() or Serial.print( LCD_CLEAR )
///              add special char CLEAR HOME CLREOL and handle /r /n in print  so println works to go next line

# define LCD_CLEAR   "\x01"  // do a fulll reinit of lcd : clear()
# define LCD_HOME    "\x02"  // fast home
# define LCD_CLREOL  "\x03"  // clear to end of line without moving cursor :)  usefull


#ifndef LiquidCrystal_PCF8574_h
#define LiquidCrystal_PCF8574_h

#include "Arduino.h"
#include "Print.h"
#include <stddef.h>
#include <stdint.h>


class LiquidCrystal_PCF8574 : public Print
{
public:
  LiquidCrystal_PCF8574(int i2cAddr);
  // note:
  // There is no sda and scl parameter for i2c in any api.
  // The Wire library has standard settings that can be overwritten by using Wire.begin(int sda, int scl) before calling LiquidCrystal_PCF8574::begin();

  // Funtions from reference:

  void begin(int cols, int rows);

  void init();

  void home();
  void setCursor(byte col, byte row);
  void setCursorLCD(byte col, byte row);
  void cursor();
  void noCursor();
  void blink();
  void noBlink();
  void display();
  void noDisplay();
  void scrollDisplayLeft();
  void scrollDisplayRight();
  void autoscroll();
  void noAutoscroll();
  void leftToRight();
  void rightToLeft();
  void createChar(int, int[]);

  // plus functions from LCDAPI:
  void clear(); // same as init()
  void setBacklight(int brightness);
  void writeLCD(uint8_t ch);   //write any char to LCD (needed to build special char)

  // support of Print class
  virtual size_t write(uint8_t ch);   //write with dealing /r /n /x01 /x02 /x03
  using Print::write;

private:
  // instance variables
  byte _i2cAddr; ///< Wire Address of the LCD
  byte _backlight; ///< the backlight intensity
  byte _lines; ///< number of lines of the display
  byte _cols;  ///< number of cols to display
  byte _row;  ///actual cursor Line position
  byte _col;   ///actual cursor Col  position
  byte _entrymode; ///<flags from entrymode
  byte _displaycontrol; ///<flags from displaycontrol
  // low level functions
  void _send(byte value, bool isData = false);
  void _sendNibble(byte halfByte, bool isData = false);
  //void _write2Wire(byte halfByte, bool isData, bool enable);
  void _writePCF(byte halfByte, bool isData);
};

#endif

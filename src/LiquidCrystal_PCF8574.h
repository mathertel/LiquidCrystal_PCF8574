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
  void setCursor(int col, int row);
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
  void createChar(int, byte[]);

  // plus functions from LCDAPI:
  void clear(); // same as init()
  void setBacklight(int brightness);

  // support of Print class
  virtual size_t write(uint8_t ch);
  using Print::write;

private:
  // instance variables
  int _i2cAddr; ///< Wire Address of the LCD
  int _backlight; ///< the backlight intensity
  int _cols; ///< number of cols of the display
  int _lines; ///< number of lines of the display
  int _entrymode; ///<flags from entrymode
  int _displaycontrol; ///<flags from displaycontrol
  int _row_offsets[4];
  
  // low level functions
  void _send(int value, bool isData = false);
  void _sendNibble(int halfByte, bool isData = false);
  void _write2Wire(int halfByte, bool isData, bool enable);
};

#endif

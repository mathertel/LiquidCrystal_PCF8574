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
/// * 16.12.2020 modified to provide user's chosen I2C hardware, changed int to unint8_t

#ifndef LiquidCrystal_PCF8574_h
#define LiquidCrystal_PCF8574_h

#include "Arduino.h"
#include <Wire.h>

#include "Print.h"
#include <stddef.h>
#include <stdint.h>


class LiquidCrystal_PCF8574 : public Print
{
public:
  LiquidCrystal_PCF8574(uint8_t i2cAddr);
  // note:
  // There is no sda and scl parameter for i2c in any api.
  // The Wire library has standard settings that can be overwritten by using Wire.begin(sda, scl) before calling LiquidCrystal_PCF8574::begin();
  // When using multiple I2C ports one can use 
  //   LiquidCrystal_PCF8574 lcd(0x27);
  //   TwoWire Wire_1 = TwoWire(); 
  //   Wire_1.begin(sdaPin, sclPin); 
  //   Wire_1.setClock(100000);             // standard 100kHz speed
  //   Wire_1.setClockStretchLimit(200000); // 200ms, for slow devices
  //   lcd.begin(clos, rows, Wire_1);

  // Funtions from reference:
  void begin(uint8_t cols, uint8_t rows, TwoWire &wirePort = Wire);

  void init();

  void home();
  void setCursor(uint8_t col, uint8_t row);
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
  void createChar(uint8_t, byte[]);

  // plus functions from LCDAPI:
  void clear(); // same as init()
  void setBacklight(uint8_t brightness);

  // support of Print class
  virtual size_t write(uint8_t ch);
  using Print::write;

private:
  TwoWire * _i2cPort; //The generic connection to user's chosen I2C hardware
  // instance variables
  uint8_t _i2cAddr;
  uint8_t _backlight; ///< the backlight intensity
  uint8_t _cols; ///< number of cols of the display
  uint8_t _lines; ///< number of lines of the display
  uint8_t _entrymode; ///<flags from entrymode
  uint8_t _displaycontrol; ///<flags from displaycontrol
  uint8_t _row_offsets[4];
  
  // low level functions
  void _send(uint8_t value, bool isData = false);
  void _sendNibble(uint8_t halfByte, bool isData = false);
  void _write2Wire(uint8_t halfByte, bool isData, bool enable);
};

#endif

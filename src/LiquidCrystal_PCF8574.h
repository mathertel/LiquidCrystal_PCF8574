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
/// This library can drive a Liquid Crystal Display (LCD) based on the Hitachi HD44780 chip that is connected 
/// through a PCF8574 I2C adapter. It uses the original Wire library for communication.
/// The API if common to many LCD libraries and documented in https://www.arduino.cc/en/Reference/LiquidCrystal.
/// and partially functions from https://playground.arduino.cc/Code/LCDAPI/.

///
/// ChangeLog:
/// --------
/// * 19.10.2013 created.
/// * 05.06.2019 rewrite from scratch.
/// * 26.05.2022 8-bit datatypes in interfaces and compatibility topics.
/// * 26.05.2022 createChar with PROGMEM character data for AVR processors.
/// * 26.05.2022 constructor with pin assignments. Thanks to @markisch.

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
  LiquidCrystal_PCF8574(uint8_t i2cAddr=0x27);
  // note:
  //
	// When using multiple I2C ports one can initialize with
  //   LiquidCrystal_PCF8574 lcd(0x27);     // create lcd
  //   TwoWire myWire = TwoWire();          // create new wire instance
  //   myWire.begin(sdaPin, sclPin);        // define SDA and SCL pins
  //   myWire.setClock(100000);             // I2C speed 100kHz
  //   myWire.setClockStretchLimit(200000); // I2C clock stretch to 200ms for slow devices
  //   lcd_port = &myWire;                  // Keep address to wire instance
  //   lcd.begin(clos, rows, *lcd_port);    // Initialize lcd
  //
  // and in the main program one updates display with
  //   # if defined(ESP8266)
  //     ESP8266 is special case because there is only one wire structure available
  //     Although we can created multiple wire interfaces we still need to specify SDA and SCL 
  //     before each transmission:
  //     lcd_port->begin(sdaPin, sclPin);     
  //     lcd_port->setClock(100000);
  //     lcd_port->setClockStretchLimit(200000); // 200ms, for slow devices
  //   # endif
  //   lcd.setCursor(0, 0); 
  //   lcd.print(lcdbuf);

  // constructors, which allow to redefine bit assignments in case your adapter is wired differently
  LiquidCrystal_PCF8574(uint8_t i2cAddr, uint8_t rs, uint8_t enable,
    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t backlight=255);
  LiquidCrystal_PCF8574(uint8_t i2cAddr, uint8_t rs, uint8_t rw, uint8_t enable,
    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t backlight=255);

  // Functions from reference:

  void begin(uint8_t cols, uint8_t rows, TwoWire &wirePort = Wire);

  void clear();
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
  void createChar(uint8_t location, uint8_t charmap[]);
#ifdef __AVR__
  void createChar_P(uint8_t, const uint8_t *);
  inline void createChar(uint8_t n, const uint8_t *data) {
    createChar_P(n, data);
  };
#endif

  // plus functions from LCDAPI:
  void setBacklight(uint8_t brightness);
  inline void command(uint8_t value) { _send(value); }

  // support of Print class
  virtual size_t write(uint8_t ch);

private:

  TwoWire *_i2cPort; //The generic connection to user's chosen I2C hardware

  // instance variables
  uint8_t _i2cAddr; ///< Wire Address of the LCD
  uint8_t _backlight; ///< the backlight intensity
  uint8_t _cols; ///< number of cols of the display
  uint8_t _lines; ///< number of lines of the display
  uint8_t _entrymode; ///<flags from entrymode
  uint8_t _displaycontrol; ///<flags from displaycontrol
  uint8_t _row_offsets[4];

  // variables describing how the PCF8574 is connected to the LCD
  uint8_t _rs_mask;
  uint8_t _rw_mask;
  uint8_t _enable_mask;
  uint8_t _backlight_mask;
  // these are used for 4-bit data to the display.
  uint8_t _data_mask[4];

  // low level functions
  void _send(uint8_t value, bool isData = false);
  void _sendNibble(uint8_t halfByte, bool isData = false);
  void _writeNibble(uint8_t halfByte, bool isData);
  void _write2Wire(uint8_t data, bool isData, bool enable);

  void init(uint8_t i2cAddr, uint8_t rs, uint8_t rw, uint8_t enable,
    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t backlight=255);
};

#endif

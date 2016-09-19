///
/// \file LiquidCrystal_PCF8574.cpp
/// \brief LiquidCrystal (LCD) library with PCF8574 I2C adapter.
///
/// \author Matthias Hertel, http://www.mathertel.de
/// \copyright Copyright (c) 2014 by Matthias Hertel.\n
/// This work is licensed under a BSD style license.\n
/// See http://www.mathertel.de/License.aspx
/// 
/// \details
/// This is a library for driving LiquidCrystal displays (LCD) by using the I2C bus and an PCF8574 I2C adapter.
/// This library is derived from the original Arduino LiquidCrystal library and uses the original Wire library for communication.
///
/// More documentation and source code is available at http://www.mathertel.de/Arduino
///
/// ChangeLog see: LiquidCrystal_PCF8574.h

#include "LiquidCrystal_PCF8574.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"
#include <Wire.h>

/// Definitions on how the PCF8574 is connected to the LCD

/// These are Bit-Masks for the special signals and background light
#define PCF_RS  0x01
#define PCF_RW  0x02
#define PCF_EN  0x04
#define PCF_BACKLIGHT 0x08

// Definitions on how the PCF8574 is connected to the LCD
// These are Bit-Masks for the special signals and Background
#define RSMODE_CMD  0
#define RSMODE_DATA 1


// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//    DL = 1; 8-bit interface data 
//    N = 0; 1-line display 
//    F = 0; 5x8 dot character font 
// 3. Display on/off control: 
//    D = 0; Display off 
//    C = 0; Cursor off 
//    B = 0; Blinking off 
// 4. Entry mode set: 
//    I/D = 1; Increment by 1 
//    S = 0; No shift 
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

// modification:
// don't use ports from Arduino, but use ports from Wire

// a nibble is a half Byte

// NEW: http://playground.arduino.cc//Code/LCDAPI
// NEW: setBacklight


LiquidCrystal_PCF8574::LiquidCrystal_PCF8574(uint8_t addr)
{
  _Addr = addr;
  _backlight = 0;
} // LiquidCrystal_PCF8574


void LiquidCrystal_PCF8574::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
  // cols ignored !
  _numlines = lines;

  _displayfunction = 0;

  if (lines > 1) {
    _displayfunction |= LCD_2LINE;
  }

  // for some 1 line displays you can select a 10 pixel high font
  if ((dotsize != 0) && (lines == 1)) {
    _displayfunction |= LCD_5x10DOTS;
  }

  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way befor 4.5V so we'll wait 50
  Wire.begin();

  // initializing th display
  _write2Wire(0x00, LOW, false);
  delayMicroseconds(50000); 

  // put the LCD into 4 bit mode according to the hitachi HD44780 datasheet figure 26, pg 47
  _sendNibble(0x03, RSMODE_CMD);
  delayMicroseconds(4500); 
  _sendNibble(0x03, RSMODE_CMD);
  delayMicroseconds(4500); 
  _sendNibble(0x03, RSMODE_CMD);
  delayMicroseconds(150);
  // finally, set to 4-bit interface
  _sendNibble(0x02, RSMODE_CMD);

  // finally, set # lines, font size, etc.
  _command(LCD_FUNCTIONSET | _displayfunction);  

  // turn the display on with no cursor or blinking default
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
  display();

  // clear it off
  clear();

  // Initialize to default text direction (for romance languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  _command(LCD_ENTRYMODESET | _displaymode);
}

/********** high level commands, for the user! */
void LiquidCrystal_PCF8574::clear()
{
  _command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void LiquidCrystal_PCF8574::home()
{
  _command(LCD_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}


/// Set the cursor to a new position. 
void LiquidCrystal_PCF8574::setCursor(uint8_t col, uint8_t row)
{
  int row_offsets[] = { 0x00, 0x40, 0x14, 0x54   };
  if ( row >= _numlines ) {
    row = _numlines-1;    // we count rows starting w/0
  }

  _command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void LiquidCrystal_PCF8574::noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  _command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_PCF8574::display() {
  _displaycontrol |= LCD_DISPLAYON;
  _command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void LiquidCrystal_PCF8574::noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  _command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_PCF8574::cursor() {
  _displaycontrol |= LCD_CURSORON;
  _command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void LiquidCrystal_PCF8574::noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  _command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_PCF8574::blink() {
  _displaycontrol |= LCD_BLINKON;
  _command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void LiquidCrystal_PCF8574::scrollDisplayLeft(void) {
  _command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LiquidCrystal_PCF8574::scrollDisplayRight(void) {
  _command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LiquidCrystal_PCF8574::leftToRight(void) {
  _displaymode |= LCD_ENTRYLEFT;
  _command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void LiquidCrystal_PCF8574::rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  _command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void LiquidCrystal_PCF8574::autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  _command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void LiquidCrystal_PCF8574::noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  _command(LCD_ENTRYMODESET | _displaymode);
}


/// Setting the brightness of the background display light.
/// The backlight can be switched on and off.
/// The current brightness is stored in the private _backlight variable to have it available for further data transfers.
void LiquidCrystal_PCF8574::setBacklight(uint8_t brightness) {
  _backlight = brightness;
  // send no data but set the background-pin right;
  _write2Wire(0x00, RSMODE_DATA, false);
} // setBacklight


// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LiquidCrystal_PCF8574::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  _command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    write(charmap[i]);
  }
}

/* The write function is needed for derivation from the Print class. */
inline size_t LiquidCrystal_PCF8574::write(uint8_t value) {
  _send(value, RSMODE_DATA);
  return 1; // assume sucess
}

/* ----- low level functions ----- */

inline void LiquidCrystal_PCF8574::_command(uint8_t value) {
  _send(value, RSMODE_CMD);
} // _command()


// write either command or data
void LiquidCrystal_PCF8574::_send(uint8_t value, uint8_t mode) {
  // separate the 4 value-nibbles
  uint8_t valueLo = value    & 0x0F;
  uint8_t valueHi = value>>4 & 0x0F;

  _sendNibble(valueHi, mode);
  _sendNibble(valueLo, mode);
} // _send()


// write a nibble / halfByte with handshake
void LiquidCrystal_PCF8574::_sendNibble(uint8_t halfByte, uint8_t mode) {
  _write2Wire(halfByte, mode, true);
  delayMicroseconds(1);    // enable pulse must be >450ns
  _write2Wire(halfByte, mode, false);
  delayMicroseconds(37);   // commands need > 37us to settle
} // _sendNibble


// private function to change the PCF8674 pins to the given value
void LiquidCrystal_PCF8574::_write2Wire(uint8_t halfByte, uint8_t mode, uint8_t enable) {
  // map the given values to the hardware of the I2C schema
  uint8_t i2cData = halfByte << 4;
  if (mode > 0) i2cData |= PCF_RS;
  // PCF_RW is never used.
  if (enable > 0) i2cData |= PCF_EN;
  if (_backlight > 0) i2cData |= PCF_BACKLIGHT;

  Wire.beginTransmission(_Addr);
  Wire.write(i2cData);
  Wire.endTransmission();   
} // write2Wire


// The End.

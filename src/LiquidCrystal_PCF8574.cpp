/// \file LiquidCrystal_PCF8574.cpp
/// \brief LiquidCrystal library with PCF8574 I2C adapter.
///
/// \author Matthias Hertel, http://www.mathertel.de
/// \copyright Copyright (c) 2019 by Matthias Hertel.
///
/// ChangeLog see: LiquidCrystal_PCF8574.h

#include "LiquidCrystal_PCF8574.h"

#include <Wire.h>

LiquidCrystal_PCF8574::LiquidCrystal_PCF8574(uint8_t i2cAddr)
{
  // default pin assignment
  init(i2cAddr, 0, 1, 2, 4, 5, 6, 7, 3);
} // LiquidCrystal_PCF8574

// constructors, which allows to redefine bit assignments in case your adapter is wired differently
LiquidCrystal_PCF8574::LiquidCrystal_PCF8574(uint8_t i2cAddr, uint8_t rs, uint8_t enable,
    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t backlight)
{
  init(i2cAddr, rs, 255, enable, d4, d5, d6, d7, backlight);
} // LiquidCrystal_PCF8574

LiquidCrystal_PCF8574::LiquidCrystal_PCF8574(uint8_t i2cAddr, uint8_t rs, uint8_t rw, uint8_t enable,
    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t backlight)
{
  init(i2cAddr, rs, rw, enable, d4, d5, d6, d7, backlight);
} // LiquidCrystal_PCF8574

void LiquidCrystal_PCF8574::init(uint8_t i2cAddr, uint8_t rs, uint8_t rw, uint8_t enable,
    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t backlight)
{
  _i2cAddr = i2cAddr;
  _backlight = 0;

  _entrymode = 0x02; // like Initializing by Internal Reset Circuit
  _displaycontrol = 0x04;

  _rs_mask = 0x01 << rs;
  if (rw != 255)
    _rw_mask = 0x01 << rw;
  else
    _rw_mask = 0;
  _enable_mask = 0x01 << enable;
  _data_mask[0] = 0x01 << d4;
  _data_mask[1] = 0x01 << d5;
  _data_mask[2] = 0x01 << d6;
  _data_mask[3] = 0x01 << d7;

  if (backlight != 255)
    _backlight_mask = 0x01 << backlight;
  else
    _backlight_mask = 0;
} // init()


void LiquidCrystal_PCF8574::begin(uint8_t cols, uint8_t lines, TwoWire &wirePort)
{
  _i2cPort = &wirePort; //Grab which port the user wants us to use

  _cols = min(cols, (uint8_t)80);
  _lines = min(lines, (uint8_t)4);

  uint8_t functionFlags = 0;

  _row_offsets[0] = 0x00;
  _row_offsets[1] = 0x40;
  _row_offsets[2] = 0x00 + cols;
  _row_offsets[3] = 0x40 + cols;

  if (lines > 1) {
    functionFlags |= 0x08;
  }

  // initializing the display
  _i2cPort->begin();
  _write2Wire(0x00, LOW, false);
  delayMicroseconds(50000);

  // after reset the mode is this
  _displaycontrol = 0x04;
  _entrymode = 0x02;

  // sequence to reset. see "Initializing by Instruction" in datasheet
  _sendNibble(0x03);
  delayMicroseconds(4500);
  _sendNibble(0x03);
  delayMicroseconds(200);
  _sendNibble(0x03);
  delayMicroseconds(200);
  _sendNibble(0x02); // finally, set to 4-bit interface

  // Instruction: Function set = 0x20
  _send(0x20 | functionFlags);

  display();
  clear();
  leftToRight();
} // begin()


void LiquidCrystal_PCF8574::clear()
{
  // Instruction: Clear display = 0x01
  _send(0x01);
  delayMicroseconds(1600); // this command takes 1.5ms!
} // clear()


void LiquidCrystal_PCF8574::home()
{
  // Instruction: Return home = 0x02
  _send(0x02);
  delayMicroseconds(1600); // this command takes 1.5ms!
} // home()


/// Set the cursor to a new position.
void LiquidCrystal_PCF8574::setCursor(uint8_t col, uint8_t row)
{
  // check boundaries
  if ((col < _cols) && (row < _lines)) {
    // Instruction: Set DDRAM address = 0x80
    _send(0x80 | (_row_offsets[row] + col));
  }
} // setCursor()


// Turn the display on/off (quickly)
void LiquidCrystal_PCF8574::noDisplay()
{
  // Instruction: Display on/off control = 0x08
  _displaycontrol &= ~0x04; // display
  _send(0x08 | _displaycontrol);
} // noDisplay()


void LiquidCrystal_PCF8574::display()
{
  // Instruction: Display on/off control = 0x08
  _displaycontrol |= 0x04; // display
  _send(0x08 | _displaycontrol);
} // display()


// Turns the underline cursor on/off
void LiquidCrystal_PCF8574::cursor()
{
  // Instruction: Display on/off control = 0x08
  _displaycontrol |= 0x02; // cursor
  _send(0x08 | _displaycontrol);
} // cursor()


void LiquidCrystal_PCF8574::noCursor()
{
  // Instruction: Display on/off control = 0x08
  _displaycontrol &= ~0x02; // cursor
  _send(0x08 | _displaycontrol);
} // noCursor()


// Turn on and off the blinking cursor
void LiquidCrystal_PCF8574::blink()
{
  // Instruction: Display on/off control = 0x08
  _displaycontrol |= 0x01; // blink
  _send(0x08 | _displaycontrol);
} // blink()


void LiquidCrystal_PCF8574::noBlink()
{
  // Instruction: Display on/off control = 0x08
  _displaycontrol &= ~0x01; // blink
  _send(0x08 | _displaycontrol);
} // noBlink()


// These commands scroll the display without changing the RAM
void LiquidCrystal_PCF8574::scrollDisplayLeft(void)
{
  // Instruction: Cursor or display shift = 0x10
  // shift: 0x08, left: 0x00
  _send(0x10 | 0x08 | 0x00);
} // scrollDisplayLeft()


void LiquidCrystal_PCF8574::scrollDisplayRight(void)
{
  // Instruction: Cursor or display shift = 0x10
  // shift: 0x08, right: 0x04
  _send(0x10 | 0x08 | 0x04);
} // scrollDisplayRight()


// == controlling the entrymode

// This is for text that flows Left to Right
void LiquidCrystal_PCF8574::leftToRight(void)
{
  // Instruction: Entry mode set, set increment/decrement =0x02
  _entrymode |= 0x02;
  _send(0x04 | _entrymode);
} // leftToRight()


// This is for text that flows Right to Left
void LiquidCrystal_PCF8574::rightToLeft(void)
{
  // Instruction: Entry mode set, clear increment/decrement =0x02
  _entrymode &= ~0x02;
  _send(0x04 | _entrymode);
} // rightToLeft()


// This will 'right justify' text from the cursor
void LiquidCrystal_PCF8574::autoscroll(void)
{
  // Instruction: Entry mode set, set shift S=0x01
  _entrymode |= 0x01;
  _send(0x04 | _entrymode);
} // autoscroll()


// This will 'left justify' text from the cursor
void LiquidCrystal_PCF8574::noAutoscroll(void)
{
  // Instruction: Entry mode set, clear shift S=0x01
  _entrymode &= ~0x01;
  _send(0x04 | _entrymode);
} // noAutoscroll()


/// Setting the brightness of the background display light.
/// The backlight can be switched on and off.
/// The current brightness is stored in the private _backlight variable to have it available for further data transfers.
void LiquidCrystal_PCF8574::setBacklight(uint8_t brightness)
{
  _backlight = brightness;
  // send no data but set the background-pin right;
  _write2Wire(0x00, true, false);
} // setBacklight()


// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LiquidCrystal_PCF8574::createChar(uint8_t location, uint8_t charmap[])
{
  location &= 0x7; // we only have 8 locations 0-7
  // Set CGRAM address
  _send(0x40 | (location << 3));
  for (uint8_t i = 0; i < 8; i++) {
    write(charmap[i]);
  }
} // createChar()


#ifdef __AVR__
// Allows us to fill the first 8 CGRAM locations
// with custom characters stored in PROGMEM
void LiquidCrystal_PCF8574::createChar_P(uint8_t location, const uint8_t *charmap) {
  PGM_P p = reinterpret_cast<PGM_P>(charmap);
  location &= 0x7; // we only have 8 locations 0-7
  _send(0x40 | (location << 3));
  for (int i = 0; i < 8; i++) {
    uint8_t c = pgm_read_byte(p++);
    write(c);
  }
} // createChar_P()
#endif


/* The write function is needed for derivation from the Print class. */
inline size_t LiquidCrystal_PCF8574::write(uint8_t ch)
{
  _send(ch, true);
  return 1; // assume success
} // write()


// write either command or data
void LiquidCrystal_PCF8574::_send(uint8_t value, bool isData)
{
  // An I2C transmission has a significant overhead of ~10+1 I2C clock
  // cycles. We consequently only perform it only once per _send().

  _i2cPort->beginTransmission(_i2cAddr);
  // write high 4 bits
  _writeNibble((value >> 4 & 0x0F), isData);
  // write low 4 bits
  _writeNibble((value & 0x0F), isData);
  _i2cPort->endTransmission();
} // _send()


// write a nibble / halfByte with handshake
void LiquidCrystal_PCF8574::_writeNibble(uint8_t halfByte, bool isData)
{
  // map the data to the given pin connections
  uint8_t data = isData ? _rs_mask : 0;
  // _rw_mask is not used here.
  if (_backlight > 0)
    data |= _backlight_mask;

  // allow for arbitrary pin configuration
  if (halfByte & 0x01) data |= _data_mask[0];
  if (halfByte & 0x02) data |= _data_mask[1];
  if (halfByte & 0x04) data |= _data_mask[2];
  if (halfByte & 0x08) data |= _data_mask[3];

  // Note that the specified speed of the PCF8574 chip is 100KHz.
  // Transmitting a single byte takes 9 clock ticks at 100kHz -> 90us.
  // The 37us delay is only necessary after sending the second nibble.
  // But in that case we have to restart the transfer using additional 
  // >10 clock cycles. Hence, no additional delays are necessary even
  // when the I2C bus is operated beyond the chip's spec in fast mode
  // at 400 kHz.

  _i2cPort->write(data | _enable_mask);
  // delayMicroseconds(1); // enable pulse must be >450ns
  _i2cPort->write(data);
  // delayMicroseconds(37); // commands need > 37us to settle
} // _writeNibble


// write a nibble / halfByte with handshake
void LiquidCrystal_PCF8574::_sendNibble(uint8_t halfByte, bool isData)
{
  _i2cPort->beginTransmission(_i2cAddr);
  _writeNibble(halfByte, isData);
  _i2cPort->endTransmission();
} // _sendNibble


// private function to change the PCF8574 pins to the given value
void LiquidCrystal_PCF8574::_write2Wire(uint8_t data, bool isData, bool enable)
{
  if (isData)
    data |= _rs_mask;
  // _rw_mask is not used here.
  if (enable)
    data |= _enable_mask;
  if (_backlight > 0)
    data |= _backlight_mask;

  _i2cPort->beginTransmission(_i2cAddr);
  _i2cPort->write(data);
  _i2cPort->endTransmission();
} // write2Wire

// The End.

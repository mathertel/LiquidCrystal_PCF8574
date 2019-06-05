/// \file LiquidCrystal_PCF8574.cpp
/// \brief LiquidCrystal library with PCF8574 I2C adapter.
///
/// \author Matthias Hertel, http://www.mathertel.de
/// \copyright Copyright (c) 2019 by Matthias Hertel.
///
/// ChangeLog see: LiquidCrystal_PCF8574.h

#include "LiquidCrystal_PCF8574.h"

#include "Arduino.h"
#include <Wire.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

/// Definitions on how the PCF8574 is connected to the LCD

/// These are Bit-Masks for the special signals and background light
#define PCF_RS 0x01
#define PCF_RW 0x02
#define PCF_EN 0x04
#define PCF_BACKLIGHT 0x08

// Definitions on how the PCF8574 is connected to the LCD
// These are Bit-Masks for the special signals and Background
#define RSMODE_CMD 0
#define RSMODE_DATA 1

// modification:
// don't use ports from Arduino, but use ports from Wire

// a nibble is a half Byte

LiquidCrystal_PCF8574::LiquidCrystal_PCF8574(int i2cAddr)
{
  _i2cAddr = i2cAddr;
  _backlight = 0;
} // LiquidCrystal_PCF8574


void LiquidCrystal_PCF8574::begin(int cols, int lines)
{
  // not implemented yet
}

/********** high level commands, for the user! */
void LiquidCrystal_PCF8574::clear()
{
  // not implemented yet
}

void LiquidCrystal_PCF8574::home()
{
  // not implemented yet
}


/// Set the cursor to a new position.
void LiquidCrystal_PCF8574::setCursor(int col, int row)
{
  // not implemented yet
}

// Turn the display on/off (quickly)
void LiquidCrystal_PCF8574::noDisplay()
{
  // not implemented yet
}

void LiquidCrystal_PCF8574::display()
{
  // not implemented yet
}

// Turns the underline cursor on/off
void LiquidCrystal_PCF8574::noCursor()
{
  // not implemented yet
}

void LiquidCrystal_PCF8574::cursor()
{
  // not implemented yet
}

// Turn on and off the blinking cursor
void LiquidCrystal_PCF8574::noBlink()
{
  // not implemented yet
}

void LiquidCrystal_PCF8574::blink()
{
  // not implemented yet
}

// These commands scroll the display without changing the RAM
void LiquidCrystal_PCF8574::scrollDisplayLeft(void)
{
  // not implemented yet
}

void LiquidCrystal_PCF8574::scrollDisplayRight(void)
{
  // not implemented yet
}

// This is for text that flows Left to Right
void LiquidCrystal_PCF8574::leftToRight(void)
{
  // not implemented yet
}

// This is for text that flows Right to Left
void LiquidCrystal_PCF8574::rightToLeft(void)
{
  // not implemented yet
}

// This will 'right justify' text from the cursor
void LiquidCrystal_PCF8574::autoscroll(void)
{
  // not implemented yet
}

// This will 'left justify' text from the cursor
void LiquidCrystal_PCF8574::noAutoscroll(void)
{
  // not implemented yet
}


/// Setting the brightness of the background display light.
/// The backlight can be switched on and off.
/// The current brightness is stored in the private _backlight variable to have it available for further data transfers.
void LiquidCrystal_PCF8574::setBacklight(int brightness)
{
  // not implemented yet
}


// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LiquidCrystal_PCF8574::createChar(int location, int charmap[])
{
  // not implemented yet
}

/* The write function is needed for derivation from the Print class. */
inline size_t LiquidCrystal_PCF8574::write(uint8_t ch)
{
  // not implemented yet
}

// The End.

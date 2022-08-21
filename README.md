# Arduino Library for LiquidCrystal displays with I2C PCF8574 adapter

[![Arduino Library Checks](https://github.com/mathertel/LiquidCrystal_PCF8574/actions/workflows/arduino-checks.yml/badge.svg)](https://github.com/mathertel/LiquidCrystal_PCF8574/actions/workflows/arduino-checks.yml)

[![arduino-library-badge](https://www.ardu-badge.com/badge/LiquidCrystal_PCF8574.svg?)](https://www.ardu-badge.com/LiquidCrystal_PCF8574)

[![GitLicense](https://gitlicense.com/badge/mathertel/LiquidCrystal_PCF8574)](https://gitlicense.com/license/mathertel/LiquidCrystal_PCF8574)

A library for driving LiquidCrystal displays (LCD) by using the I2C bus and an PCF8574 I2C adapter.

There are modules that can be soldered or stacked to the display that offers an I2C interface for communication instead of the 8+ digital lines that are used to send data to the display.

Most of these modules use the wiring that is supported by this library's default configuration.
If you use a module with a different wiring, you can use one of the class constructors which allow you to specify the pin assignments.

See the web site for more details and pictures: <https://www.mathertel.de/Arduino/LiquidCrystal_PCF8574.aspx>

## Changes Version 2.1.0 (2022-08-21)

Enables using a non-default i2c / TwoWire port for boards that support multiple ports.

Added example `LCD_Burnin.ino` showing how to transfer a 20 char and 4 lines buffer to a device all at once.

Thanks for additions from @uutzinger


## Changes Version 2.0.0 (2022-05-26)

* The interface was modified to use 8-bit datatypes as supported by the underling hardware.
* A createChar version with PROGMEM character data for AVR processors was added.
* A constructor with pin assignments was added to allow easy adoption to other wirings than the default.

Thanks for additions from @markisch.

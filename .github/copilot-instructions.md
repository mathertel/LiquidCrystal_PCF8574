# Copilot Instructions for LiquidCrystal_PCF8574

This document guides GitHub Copilot on how to assist with development on the LiquidCrystal_PCF8574 Arduino library.

## Project Expectations

This is an **Arduino library** that supports:

* Arduino boards (Uno, Mega, Nano, etc.)
* ESP8266 microcontrollers
* ESP32 microcontrollers

The library interfaces with LCD displays (typically 16x2 or 20x4) via PCF8574 I2C port
expanders, enabling seamless text display with minimal pin usage.

**Key Principles:**

* Multi-platform compatibility across all Arduino cores
* Production-ready codebase suitable for embedded systems
* Backward API compatibility is mandatory for new versions
* Minimal resource footprint for memory-constrained devices

## Coding Style

### General Style

* **Language:** C++ (Arduino flavor)
* **Indentation:** 2 spaces
* **Line Length:** Keep under 100 characters where practical
* **Case Convention:**
  * Class/type names: `PascalCase`
  * Function/method names: `camelCase`
  * Constants and macros: `UPPER_CASE`
  * Member variables: `_camelCase` (with underscore prefix for private members)

### Arduino-Specific Conventions

* Use `setup()` and `loop()` pattern for sketches
* Prefer `Serial.print()` for debug output
* Use `delay()` sparingly; prefer non-blocking patterns
* Initialize class instances as globals or in `setup()`, not in `loop()`

### Code Quality

* Remove trailing whitespace
* Use meaningful variable names
* Add comments for complex logic
* Keep functions focused on a single responsibility
* Use header guards in `.h` files: `#ifndef LIBRARY_NAME_H` / `#define LIBRARY_NAME_H`

## Library-Specific Rules

### API Contract

* **No breaking changes** to existing public methods
* New features should extend functionality without modifying existing signatures
* Deprecated functionality must remain functional for at least one major version
* All public methods must be documented in comments

### Memory Management

* **FORBIDDEN:** `new`, `delete`, `malloc()`, and dynamic allocation
* Use static arrays, preallocated buffers, and fixed-size data structures only
* Especially critical for long-running embedded applications

### I2C Communication

* The library uses the I2C protocol via the `Wire` library
* Always handle I2C errors gracefully
* Support configurable I2C addresses
* Include timeouts where appropriate to prevent blocking indefinitely

### LCD Control

* Support standard 16x2 and 20x4 LCD formats
* Implement cursor movement, text display, and special functions
* The PCF8574 port expander controls backlighting via pin control
* Maintain compatibility with the standard LiquidCrystal API where possible

### Non-Blocking Patterns

* Avoid `delay()` in library methods; let the application control timing
* Methods should return immediately after queuing operations
* For display updates, batch operations where possible
* Document any methods that may take time to execute

### Minimal Complete Examples

* See the `examples/` directory:
  * `LCD_Burnin/LCD_Burnin.ino` — Stress test for the display
  * `LiquidCrystal_PCF8574_Test/LiquidCrystal_PCF8574_Test.ino` — Basic usage and feature demonstration
* Examples should be self-contained and compile without modification
* Demonstrate best practices for initialization and typical use cases

### Dependencies

* Only depends on the standard Arduino `Wire` library
* **FORBIDDEN:** Cloud libraries, WiFi/network dependencies, or external HTTP calls
* Must work offline and in isolated environments
* For platform-specific features (if any), guard behind `#ifdef ARDUINO_ARCH_ESP32` etc.

## How Copilot Should Respond

When assisting with:

* **Bug fixes:** Provide solutions that maintain backward compatibility
* **New features:** Suggest additive changes that extend existing methods; never replace them
* **Code review:** Emphasize memory safety and embedded-system best practices
* **Documentation:** Ensure all public APIs have inline comments explaining parameters and return values
* **Examples:** Generate self-contained sketches that demonstrate one feature clearly
* **Questions:** Reference the `src/` directory and `examples/` folder as authoritative sources
* **Performance:** Favor small memory footprint and execution speed over convenience abstractions

## Do Not

❌ **Do not:**

* Allocate memory dynamically (`new`, `delete`, `malloc()`)
* Introduce breaking changes to existing public methods
* Add dependencies on external libraries beyond Arduino's standard library
* Use blocking operations in library code without clear documentation
* Add cloud connectivity, WiFi, or network features
* Use features not available in the oldest supported Arduino core versions
* Create large local arrays on the stack (prefer class member variables)
* Ignore error handling for I2C operations
* Call `delay()` within library methods; let applications control timing
* Generate code with platform-specific behavior without `#ifdef` guards
* Bypass the public API to access private members from outside the class
* Add features without verifying compilation on ESP8266 and ESP32 cores

---

**Last Updated:** February 2026
**Scope:** All contributions to the LiquidCrystal_PCF8574 library

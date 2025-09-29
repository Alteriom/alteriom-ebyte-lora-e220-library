# Alteriom EByte LoRa E220 Series Library

[![Build Status](https://github.com/Alteriom/EByte_LoRa_E220_Series_Library/actions/workflows/build-test.yml/badge.svg)](https://github.com/Alteriom/EByte_LoRa_E220_Series_Library/actions/workflows/build-test.yml)
[![Documentation](https://github.com/Alteriom/EByte_LoRa_E220_Series_Library/actions/workflows/documentation.yml/badge.svg)](https://github.com/Alteriom/EByte_LoRa_E220_Series_Library/actions/workflows/documentation.yml)
[![Arduino Library Manager](https://img.shields.io/badge/Arduino_Library_Manager-Compatible-blue)](https://www.arduino.cc/reference/en/libraries/)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-Compatible-orange)](https://platformio.org/lib/show/15400/Alteriom_EByte_LoRa_E220)
[![NPM](https://img.shields.io/npm/v/alteriom-ebyte-lora-e220)](https://www.npmjs.com/package/alteriom-ebyte-lora-e220)

**The ultimate Arduino library for EByte LoRa E220 LLCC68 devices with 5-10km range communication support.**

## Why Choose Alteriom EByte LoRa E220

### Easy to Use
Get up and running in under 5 minutes with our comprehensive quick start guide and real-world examples.

### Multi-Platform
Works seamlessly across Arduino, ESP32, ESP8266, STM32, Raspberry Pi Pico, and SAMD platforms.

### Long Range
Achieve 5-10km communication range with the powerful LLCC68 chipset and optimized antenna setup.

### Power Efficient
Wake-on-Radio (WOR) support for ultra-low power applications and battery-powered projects.

### Secure
Built-in encryption support for secure communication in industrial and commercial applications.

### World-Class Documentation
Comprehensive documentation with interactive examples, troubleshooting guides, and platform-specific setup instructions.

## Quick Start

### Installation

**Arduino IDE:**

```bash
Tools → Manage Libraries → Search "Alteriom_EByte_LoRa_E220"
```

**PlatformIO:**

```ini
lib_deps = Alteriom/Alteriom_EByte_LoRa_E220@^1.1.6
```

### Basic Wiring

```
E220 → Arduino/ESP32
VCC  → 3.3V
GND  → GND  
RXD  → Pin 2 (or GPIO 17 on ESP32)
TXD  → Pin 3 (or GPIO 16 on ESP32)
```

### Basic Example

```arduino
#include "Alteriom_EByte_LoRa_E220.h"

LoRa_E220 e220ttl(2, 3); // RX, TX pins

void setup() {
  e220ttl.begin();
  e220ttl.sendMessage("Hello LoRa World!");
}

void loop() {
  // Your code here
}
```

## Full Documentation

**[👉 View Complete Documentation](https://alteriom.github.io/EByte_LoRa_E220_Series_Library/)**

*Build amazing LoRa projects with confidence!*
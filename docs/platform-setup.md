# Platform Setup

## Supported Platforms

The EByte LoRa E220 library supports multiple Arduino-compatible platforms:

- ✅ **Arduino** (Uno, Nano, Mega, etc.)
- ✅ **ESP32** (All variants)
- ✅ **ESP8266** (NodeMCU, Wemos D1, etc.)
- ✅ **STM32** (Nucleo, BluePill, etc.)
- ✅ **Raspberry Pi Pico**
- ✅ **SAMD** (Arduino Zero, MKR series)

## General Requirements

### Hardware
- Compatible microcontroller with UART support
- 3.3V power supply (≥250mA current capability)
- Available GPIO pins for M0, M1, AUX control

### Software
- Arduino IDE 1.8.0+ (recommended 2.0+)
- Platform-specific board packages
- EByte LoRa E220 library

## Platform-Specific Setup

### Arduino (5V Boards)

**Special Considerations**: E220 operates at 3.3V, requires voltage level conversion

**Recommended Setup**:
```cpp
#include "LoRa_E220.h"

// Use voltage dividers for 5V Arduino
LoRa_E220 e220ttl(2, 3, 4, 5, 6, UART_BPS_RATE_9600);
//              RX TX M0 M1 AX
```

**Voltage Divider Circuit**:
```
Arduino 5V → 1kΩ → E220 Pin
                 ↓
                2kΩ → GND
```

### ESP32

**Advantages**: 3.3V native, multiple UARTs, WiFi/Bluetooth combo

**Recommended Setup**:
```cpp
#include "LoRa_E220.h"

// ESP32 - use any available pins
LoRa_E220 e220ttl(&Serial2, 15, 16, 17);
//              UART  M0  M1  AUX
```

**Alternative with Software Serial**:
```cpp
#include <SoftwareSerial.h>
SoftwareSerial mySerial(16, 17); // RX, TX

LoRa_E220 e220ttl(&mySerial, 15, 2, 4);
```

### ESP8266

**Considerations**: Limited pins, software serial recommended for additional UART

**Recommended Setup**:
```cpp
#include <SoftwareSerial.h>
#include "LoRa_E220.h"

SoftwareSerial mySerial(D2, D3); // RX, TX pins

LoRa_E220 e220ttl(&mySerial, D1, D4, D5);
//              UART    M0  M1  AUX
```

### STM32

**Advantages**: Multiple UARTs, 3.3V native, high performance

**Recommended Setup**:
```cpp
#include "LoRa_E220.h"

// STM32 with Hardware Serial
LoRa_E220 e220ttl(&Serial1, PA8, PA9, PA10);
//              UART    M0   M1   AUX
```

### Raspberry Pi Pico

**Setup with Arduino Core**:
```cpp
#include "LoRa_E220.h"

// Pico - use any available pins
LoRa_E220 e220ttl(&Serial1, 2, 3, 4);
//              UART    M0 M1 AUX
```

## Pin Configuration Guidelines

### Required Connections

| E220 Pin | Function | Connection |
|----------|----------|------------|
| VCC | Power | 3.3V (⚠️ NOT 5V) |
| GND | Ground | GND |
| TX | Transmit | MCU RX pin |
| RX | Receive | MCU TX pin |
| M0 | Mode 0 | Any GPIO |
| M1 | Mode 1 | Any GPIO |
| AUX | Auxiliary | Any GPIO |

### Operating Modes

| M1 | M0 | Mode | Description |
|----|----|----- |-------------|
| 0 | 0 | Transparent | Normal transmission |
| 0 | 1 | WOR Transmit | Wake on radio TX |
| 1 | 0 | WOR Receive | Wake on radio RX |
| 1 | 1 | Configuration | Setup mode |

## Power Supply Considerations

### Voltage Requirements
- **Operating Voltage**: 2.3V - 5.2V (3.3V recommended)
- **Current Consumption**: 
  - Sleep: ~2μA
  - Receive: ~12mA
  - Transmit: 120mA (22dBm), 900mA (30dBm)

### Power Supply Design
```
Power Source → Voltage Regulator → E220 Module
     ↓              (3.3V)           ↓
  Decoupling    →  100μF + 10μF  ←  Decoupling
 Capacitors        Capacitors      Capacitors
```

## Common Setup Issues

### Issue: Module Not Responding
**Causes**:
- Incorrect voltage levels
- Wrong pin connections
- Insufficient power supply

**Solutions**:
- Verify 3.3V power supply
- Check TX/RX pin connections
- Use voltage dividers for 5V boards

### Issue: Compilation Errors
**Causes**:
- Missing board package
- Incorrect library installation

**Solutions**:
- Install platform-specific board package
- Verify library is properly installed
- Check Arduino IDE version

## Next Steps

After platform setup:
1. [Wiring Guide](wiring.md) - Physical connections
2. [Installation](installation.md) - Library setup
3. [Quick Start](quickstart.md) - First program

## Platform-Specific Guides

- [Arduino Setup](platform-arduino.md)
- [ESP32 Setup](platform-esp32.md)
- [ESP8266 Setup](platform-esp8266.md)
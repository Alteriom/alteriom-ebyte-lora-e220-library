# Wiring Guide

## Overview

This guide shows how to properly connect your EByte LoRa E220 module to various microcontrollers. **Critical**: The E220 operates at 3.3V - using 5V will damage the module!

## E220 Module Pinout

```
 ┌─────────────────┐
 │ E220 LoRa Module│
 ├─────────────────┤
 │ M0      VCC     │ ← 3.3V ONLY
 │ M1      GND     │ ← Ground
 │ RX      TX      │ ← UART pins
 │ AUX     RX      │ ← UART pins
 └─────────────────┘
```

| Pin | Function | Description |
|-----|----------|-------------|
| VCC | Power | 3.3V power input (⚠️ NOT 5V) |
| GND | Ground | Ground connection |
| TX | Transmit | Module transmit → MCU receive |
| RX | Receive | Module receive ← MCU transmit |
| AUX | Auxiliary | Status/control pin |
| M0 | Mode 0 | Operating mode control |
| M1 | Mode 1 | Operating mode control |

## Arduino Uno/Nano (5V) Wiring

**⚠️ Warning**: Arduino operates at 5V, E220 at 3.3V. Voltage dividers required!

```cpp
// Pin connections
E220 Pin → Arduino Pin
VCC      → 3.3V (from voltage regulator)
GND      → GND
TX       → Pin 2 (SoftwareSerial RX)
RX       → Pin 3 (through voltage divider)
M0       → Pin 4 (through voltage divider)
M1       → Pin 5 (through voltage divider)
AUX      → Pin 6 (direct connection OK)
```

**Voltage Divider Circuit** for RX, M0, M1 pins:
```
Arduino Pin → 1kΩ → E220 Pin
                  ↓
                 2kΩ → GND
```

**Power Supply Options**:
1. External 3.3V regulator (recommended)
2. Arduino 3.3V pin (limited current - 50mA max)

## ESP32 Wiring (Recommended)

**Advantages**: Native 3.3V, multiple hardware UARTs, no voltage conversion needed

```cpp
// Recommended ESP32 connections
E220 Pin → ESP32 Pin
VCC      → 3.3V
GND      → GND
TX       → GPIO16 (Serial2 RX)
RX       → GPIO17 (Serial2 TX)
M0       → GPIO18
M1       → GPIO19
AUX      → GPIO21
```

**Alternative Pins** (ESP32 has flexible GPIO):
```cpp
// Alternative ESP32 setup
E220 Pin → ESP32 Pin
VCC      → 3.3V
GND      → GND
TX       → GPIO25
RX       → GPIO26
M0       → GPIO32
M1       → GPIO33
AUX      → GPIO27
```

## ESP8266 Wiring

**Considerations**: Limited pins, use with care

```cpp
// ESP8266 NodeMCU connections
E220 Pin → NodeMCU Pin
VCC      → 3.3V
GND      → GND
TX       → D1 (GPIO5)
RX       → D2 (GPIO4)
M0       → D5 (GPIO14)
M1       → D6 (GPIO12)
AUX      → D7 (GPIO13)
```

**Pin Limitations**:
- Avoid D0, D3, D4 (boot/flash pins)
- D8 (GPIO15) must be LOW at boot
- Limited pins available

## STM32 Wiring

**STM32 Nucleo/BluePill** (3.3V native):
```cpp
// STM32 connections
E220 Pin → STM32 Pin
VCC      → 3.3V
GND      → GND
TX       → PA10 (USART1 RX)
RX       → PA9  (USART1 TX)
M0       → PA8
M1       → PB0
AUX      → PB1
```

## Raspberry Pi Pico Wiring

```cpp
// Pico connections
E220 Pin → Pico Pin
VCC      → 3.3V (OUT)
GND      → GND
TX       → GP1 (UART0 RX)
RX       → GP0 (UART0 TX)
M0       → GP2
M1       → GP3
AUX      → GP4
```

## Breadboard Layout Example

```
    ESP32 Dev Board          E220 Module
    ┌─────────────┐         ┌─────────────┐
    │     3.3V ●──┼─────────┼●● VCC       │
    │      GND ●──┼─────────┼●● GND       │
    │    GPIO16 ●──┼─────────┼●● TX        │
    │    GPIO17 ●──┼─────────┼●● RX        │
    │    GPIO18 ●──┼─────────┼●● M0        │
    │    GPIO19 ●──┼─────────┼●● M1        │
    │    GPIO21 ●──┼─────────┼●● AUX       │
    └─────────────┘         └─────────────┘
```

## Power Supply Requirements

### Voltage
- **Input**: 3.3V (±0.3V tolerance)
- **Never exceed**: 3.6V (will damage module)
- **Minimum**: 3.0V (below this, unreliable operation)

### Current
- **Sleep mode**: ~2μA
- **Receive mode**: ~12mA
- **Transmit mode**: 120mA (22dBm) to 900mA (30dBm)
- **Recommended supply**: ≥1A capability for safety

### Decoupling Capacitors
Add capacitors near the E220 module:
```
3.3V → 100μF electrolytic || 10μF ceramic → E220 VCC
                              ↓
                             GND
```

## Antenna Connection

### Antenna Requirements
- **Frequency**: 410-441MHz (for E220-400 series)
- **Impedance**: 50Ω
- **Connector**: U.FL/IPEX (tiny connector on module)

### Antenna Options
1. **Duck antenna**: 2-5dBi gain, omnidirectional
2. **PCB antenna**: Compact, integrated option
3. **Wire antenna**: Simple 1/4 wave = ~17cm for 433MHz

**⚠️ Never operate without antenna** - can damage the RF amplifier!

## Common Wiring Mistakes

### Voltage Issues
❌ **Wrong**: Connecting 5V to E220 VCC
✅ **Correct**: Use 3.3V or voltage regulator

❌ **Wrong**: Direct connection from 5V Arduino pins
✅ **Correct**: Use voltage dividers for signal pins

### Pin Confusion
❌ **Wrong**: Swapping TX/RX connections
✅ **Correct**: E220 TX → MCU RX, E220 RX → MCU TX

❌ **Wrong**: Forgetting M0/M1 control pins
✅ **Correct**: Connect M0/M1 for mode control

### Power Supply
❌ **Wrong**: Using Arduino 3.3V pin for high power modules
✅ **Correct**: External 3.3V regulator with adequate current

## Testing Your Wiring

### Basic Connectivity Test
```cpp
#include "LoRa_E220.h"

LoRa_E220 e220ttl(2, 3, 4, 5, 6, UART_BPS_RATE_9600);

void setup() {
  Serial.begin(9600);
  e220ttl.begin();
  
  // Test if module responds
  ResponseStructContainer c = e220ttl.getConfiguration();
  if (c.status.code == 1) {
    Serial.println("✅ E220 connected successfully!");
  } else {
    Serial.println("❌ E220 connection failed!");
    Serial.println(c.status.getResponseDescription());
  }
  c.close();
}
```

### Voltage Check
Use a multimeter to verify:
- **VCC pin**: Exactly 3.3V (±0.1V)
- **Signal pins**: 0V when LOW, 3.3V when HIGH
- **No 5V anywhere** on E220 connections

## Next Steps

After successful wiring:
1. [Installation Guide](installation.md) - Install the library
2. [Quick Start](quickstart.md) - Run your first example
3. [Configuration Guide](configuration.md) - Set up module parameters

## Troubleshooting

If wiring doesn't work:
- [Troubleshooting Guide](troubleshooting.md)
- Check all connections with multimeter
- Verify 3.3V power supply
- Test with basic connectivity code
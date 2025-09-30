# Installation Guide

## Prerequisites

- Arduino IDE 1.8.0 or newer (recommended 2.0+)
- Compatible microcontroller (Arduino, ESP32, ESP8266, STM32, Raspberry Pi Pico)
- EByte LoRa E220 module

## Installation Methods

### Method 1: Arduino Library Manager (Recommended)

1. Open Arduino IDE
2. Go to **Sketch** → **Include Library** → **Manage Libraries**
3. Search for `EByte LoRa E220`
4. Click **Install** on the library by Renzo Mischianti
5. Click **Install all** when prompted for dependencies

### Method 2: GitHub Download

1. Go to the [GitHub repository](https://github.com/xreef/EByte_LoRa_E220_Series_Library)
2. Click **Code** → **Download ZIP**
3. Extract the ZIP file
4. Copy the folder to your Arduino libraries directory:
   - **Windows**: `Documents/Arduino/libraries/`
   - **Mac**: `~/Documents/Arduino/libraries/`
   - **Linux**: `~/Arduino/libraries/`

### Method 3: Git Clone

```bash
cd ~/Documents/Arduino/libraries/
git clone https://github.com/xreef/EByte_LoRa_E220_Series_Library.git
```

### Method 4: PlatformIO

Add to your `platformio.ini`:

```ini
[env:your_board]
platform = your_platform
board = your_board
framework = arduino
lib_deps = 
    xreef/EByte LoRa E220 Series Library@^1.0.8
```

## Verification

1. Restart Arduino IDE
2. Go to **File** → **Examples** → **EByte LoRa E220 Series Library**
3. Open `01_getConfiguration`
4. Verify the library compiles without errors

## Next Steps

- [Quick Start Guide](quickstart.md) - Get up and running in 5 minutes
- [Wiring Guide](wiring.md) - Connect your E220 module
- [Examples Library](examples.md) - Explore code examples

## Troubleshooting

If installation fails:

1. **Library not found**: Check the Arduino libraries folder path
2. **Compilation errors**: Ensure you have the latest Arduino IDE
3. **Permission issues**: Run Arduino IDE as administrator (Windows) or use `sudo` (Linux)

For more help, see the [Troubleshooting Guide](troubleshooting.md).
# Alteriom EByte LoRa E220 Series Library

[![Build and Test](https://github.com/Alteriom/EByte_LoRa_E220_Series_Library/actions/workflows/build-test.yml/badge.svg)](https://github.com/Alteriom/EByte_LoRa_E220_Series_Library/actions/workflows/build-test.yml)
[![Release](https://github.com/Alteriom/EByte_LoRa_E220_Series_Library/actions/workflows/release.yml/badge.svg)](https://github.com/Alteriom/EByte_LoRa_E220_Series_Library/actions/workflows/release.yml)
[![Arduino Library Manager](https://img.shields.io/badge/Arduino_Library_Manager-Compatible-blue)](https://www.arduino.cc/reference/en/libraries/)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-Compatible-orange)](https://registry.platformio.org/libraries/alteriom/Alteriom_EByte_LoRa_E220)

This is the **Alteriom fork** of the EByte LoRa E220 Series Library, enhanced with modern CI/CD practices, automated releases, and improved package management.

## 🚀 What's New in the Alteriom Fork

- ✅ **Automated CI/CD** with GitHub Actions
- ✅ **Automatic releases** with semantic versioning
- ✅ **Arduino Library Manager** ready
- ✅ **PlatformIO** fully supported
- ✅ **NPM package** for MCP server integration
- ✅ **Comprehensive testing** across multiple platforms
- ✅ **Enhanced documentation** and examples

## 📋 Features

- **Long Range Communication**: 5-10km range with LLCC68 chipset
- **Multi-Platform Support**: Arduino, ESP32, ESP8266, STM32, Raspberry Pi Pico, SAMD
- **Power Management**: Wake-on-Radio (WOR) support for low power applications
- **Flexible Communication**: Transparent, fixed, and broadcast messaging modes
- **RSSI Monitoring**: Signal strength measurement and monitoring
- **Easy Configuration**: Simple API for device setup and management

## 🛠️ Installation

### Arduino Library Manager
1. Open Arduino IDE
2. Go to **Tools** → **Manage Libraries**
3. Search for "**Alteriom_EByte_LoRa_E220**"
4. Click **Install**

### PlatformIO
Add to your `platformio.ini`:
```ini
lib_deps = 
    Alteriom/Alteriom_EByte_LoRa_E220@^1.0.0
```

### Manual Installation
1. Download the latest release from [GitHub Releases](https://github.com/Alteriom/EByte_LoRa_E220_Series_Library/releases)
2. Extract to your Arduino libraries folder
3. Restart Arduino IDE

### NPM Package (Node.js/MCP Integration)

**NPM Registry (Public)**:
```bash
npm install alteriom-ebyte-lora-e220
```

**GitHub Packages (Requires Authentication)**:
```bash
# Configure registry
echo "@alteriom:registry=https://npm.pkg.github.com" >> .npmrc
# Install package
npm install @alteriom/alteriom-ebyte-lora-e220
```

## 🔌 Quick Start

```cpp
#include "Alteriom_EByte_LoRa_E220.h"

// Initialize with Serial pins (adjust for your board)
LoRa_E220 e220ttl(&Serial2, 15, 21, 19); // ESP32: RX, TX, AUX, M0, M1

void setup() {
    Serial.begin(9600);
    e220ttl.begin();
    
    // Get current configuration
    ResponseStructContainer c = e220ttl.getConfiguration();
    Configuration configuration = *(Configuration*) c.data;
    
    Serial.println("LoRa E220 initialized successfully!");
    c.close();
}

void loop() {
    // Send a message
    ResponseStatus rs = e220ttl.sendMessage("Hello from Alteriom LoRa!");
    
    delay(5000);
}
```

## 📚 Examples

The library includes comprehensive examples:

- **01_getConfiguration** - Read device configuration
- **01_setConfiguration** - Configure device parameters  
- **02_sendTransparentTransmission** - Simple message sending
- **04_sendFixedTransmission** - Targeted message delivery
- **05_sendFixedTransmissionStructure** - Send structured data
- **06_sendWORMessage** - Wake-on-Radio messaging
- **07_receiveMessages** - Message reception handling

## 🔧 Supported Platforms

| Platform | Framework | Status |
|----------|-----------|--------|
| Arduino UNO/Nano | Arduino | ✅ Tested |
| ESP32 | Arduino/ESP-IDF | ✅ Tested |
| ESP8266 | Arduino | ✅ Tested |
| STM32 | Arduino/STM32Cube | ✅ Tested |
| Raspberry Pi Pico | Arduino | ✅ Tested |
| SAMD21/SAMD51 | Arduino | ✅ Tested |

## 🏗️ Building and Testing

### PlatformIO
```bash
# Build for all platforms
pio run

# Run tests
pio test

# Build specific platform
pio run -e esp32dev
```

### Arduino CLI
```bash
# Compile example
arduino-cli compile --fqbn esp32:esp32:esp32 examples/01_getConfiguration/
```

## 📖 Documentation

### 🌟 Comprehensive Documentation System

This library features **world-class documentation** with:

- **📚 Complete API Reference**: Full Doxygen documentation with examples
- **🚀 Live Documentation**: Auto-generated and deployed to GitHub Pages  
- **💡 Interactive Examples**: 8+ Arduino sketches with detailed explanations
- **🔧 Platform Guides**: Specific setup instructions for each supported platform
- **⚡ Quick Start**: Get running in under 5 minutes

### 📋 Documentation Resources

- **[📖 API Documentation](https://alteriom.github.io/EByte_LoRa_E220_Series_Library/)** - Complete API reference (auto-generated)
- **[🚀 Quick Start Guide](https://alteriom.github.io/EByte_LoRa_E220_Series_Library/quickstart)** - Get up and running fast
- **[📚 Examples Library](examples/)** - 8+ complete Arduino sketches  
- **[🔧 Platform Setup](https://alteriom.github.io/EByte_LoRa_E220_Series_Library/platform-setup)** - Platform-specific configuration
- **[📋 Configuration Guide](https://alteriom.github.io/EByte_LoRa_E220_Series_Library/configuration)** - Device parameter reference
- **[🛠️ Troubleshooting](https://alteriom.github.io/EByte_LoRa_E220_Series_Library/troubleshooting)** - Common issues and solutions
- **[📊 Performance Guide](https://alteriom.github.io/EByte_LoRa_E220_Series_Library/performance)** - Range and power optimization
- **[🔒 Security Guide](https://alteriom.github.io/EByte_LoRa_E220_Series_Library/encryption)** - Encryption and secure communication

### 🎯 Documentation Features

- **✅ 100% API Coverage**: Every method documented with examples
- **✅ Multi-Platform**: Arduino, ESP32, STM32, Pi Pico specific notes  
- **✅ Real Examples**: Working code for every major feature
- **✅ Error Handling**: Comprehensive status codes and recovery
- **✅ Performance Data**: Range testing and power consumption
- **✅ Auto-Generated**: Always up-to-date with latest code changes

### 🔄 Documentation Updates

Documentation is automatically updated on every code change:

[![Documentation](https://github.com/Alteriom/EByte_LoRa_E220_Series_Library/actions/workflows/documentation.yml/badge.svg)](https://github.com/Alteriom/EByte_LoRa_E220_Series_Library/actions/workflows/documentation.yml)

**View Latest**: [https://alteriom.github.io/EByte_LoRa_E220_Series_Library/](https://alteriom.github.io/EByte_LoRa_E220_Series_Library/)

## 🤝 Contributing

We welcome contributions! Please see our [Contributing Guidelines](https://alteriom.github.io/EByte_LoRa_E220_Series_Library/contributing) for details.

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📄 License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

## 🙏 Acknowledgments

- **Renzo Mischianti** - Original author and creator of the EByte LoRa E220 library
- **Original Repository**: [xreef/EByte_LoRa_E220_Series_Library](https://github.com/xreef/EByte_LoRa_E220_Series_Library)
- **Alteriom Team** - CI/CD enhancements and modern packaging

## 🔗 Links

- **GitHub Repository**: https://github.com/Alteriom/EByte_LoRa_E220_Series_Library
- **Original Author's Website**: https://www.mischianti.org
- **Arduino Library Registry**: [Submit here](https://github.com/arduino/library-registry)
- **PlatformIO Registry**: [View package](https://registry.platformio.org/libraries/alteriom/Alteriom_EByte_LoRa_E220)

---

**Ready to get started?** Check out our [Quick Start Guide](https://alteriom.github.io/EByte_LoRa_E220_Series_Library/quickstart) and start building your LoRa projects today!
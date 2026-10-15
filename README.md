# Alteriom EByte LoRa E220 Series Library

[![Build and Test](https://github.com/Alteriom/alteriom-ebyte-lora-e220-library/actions/workflows/build-test.yml/badge.svg)](https://github.com/Alteriom/alteriom-ebyte-lora-e220-library/actions/workflows/build-test.yml)
[![Release](https://github.com/Alteriom/alteriom-ebyte-lora-e220-library/actions/workflows/release.yml/badge.svg)](https://github.com/Alteriom/alteriom-ebyte-lora-e220-library/actions/workflows/release.yml)
[![Arduino Library Manager](https://img.shields.io/badge/Arduino_Library_Manager-Compatible-blue)](https://www.arduino.cc/reference/en/libraries/)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-Compatible-orange)](https://registry.platformio.org/libraries/alteriom/Alteriom_EByte_LoRa_E220)

---

## 📚 **[🚀 VIEW COMPLETE DOCUMENTATION](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/)**

**Experience our modern, interactive documentation with live search, mobile-friendly design, and copy-paste Arduino examples!**

[![Documentation Preview](https://img.shields.io/badge/📚_Modern_Docs-Live_Now-success?style=for-the-badge&logo=gitbook&logoColor=white)](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/)
[![Quick Start](https://img.shields.io/badge/⚡_Quick_Start-5_Minutes-blue?style=for-the-badge&logo=arduino&logoColor=white)](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/quickstart)
[![API Reference](https://img.shields.io/badge/🔧_API_Docs-Complete-green?style=for-the-badge&logo=cplusplus&logoColor=white)](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/api-reference)
[![Doxygen API](https://img.shields.io/badge/🔬_Doxygen_API-Detailed-orange?style=for-the-badge&logo=doxygen&logoColor=white)](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/doxygen-api)

---

This is the **Alteriom fork** of the EByte LoRa E220 Series Library, enhanced with modern CI/CD practices, automated releases, and improved package management.

## 🤝 Contributing

We welcome contributions! Please see our [Contributing Guidelines](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/CONTRIBUTING) for details.

## 📖 Community Wiki

Our GitHub Wiki provides community-driven content that supplements the main documentation:

> 💡 **Note**: For complete documentation and getting started guides, visit our **[📚 Modern Documentation Site](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/)**

- **[📖 Visit the Wiki](https://github.com/Alteriom/alteriom-ebyte-lora-e220-library/wiki)** - Community guides and tutorials
- **[🤝 Community Guides](https://github.com/Alteriom/alteriom-ebyte-lora-e220-library/wiki/Community-Guides)** - User-contributed content
- **[📋 Wiki Integration Guide](docs/wiki-integration-guide.md)** - How to contribute to the wiki

> 💡 **Tip**: For the most comprehensive and up-to-date documentation, start with our **[📚 Live Documentation](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/)**

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
1. Download the latest release from [GitHub Releases](https://github.com/Alteriom/alteriom-ebyte-lora-e220-library/releases)
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

### 🚀 **[📚 VIEW LIVE DOCUMENTATION →](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/)**

**Experience our modern, interactive documentation with:**

✨ **Beautiful responsive design** - Works perfectly on desktop, tablet, and mobile  
🔍 **Live search** - Find what you need instantly  
💡 **Interactive examples** - Copy-paste ready Arduino code  
🎯 **Guided tutorials** - Step-by-step instructions for every platform  
📱 **Mobile-friendly** - Read docs anywhere, anytime  
⚡ **Fast loading** - Optimized for quick access  

---

### 🌟 Modern Documentation Features

This library features **world-class documentation** built with modern web technologies:

- **📚 Complete API Reference**: Interactive documentation with live examples
- **🚀 Modern Web Interface**: Beautiful, responsive design powered by Docsify  
- **💡 Step-by-Step Guides**: 8+ Arduino sketches with detailed explanations
- **🔧 Platform-Specific Setup**: Tailored instructions for each supported platform
- **⚡ 5-Minute Quick Start**: Get your first LoRa message working fast
- **🔍 Full-Text Search**: Find any information instantly
- **📱 Mobile Optimized**: Perfect experience on any device

### 📋 Quick Access Links

| Section | Description | Link |
|---------|-------------|------|
| 🏠 **Home** | Main documentation portal | **[📚 Live Docs](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/)** |
| 🚀 **Quick Start** | Get running in 5 minutes | **[⚡ Start Here](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/quickstart)** |
| 📖 **API Reference** | Complete method documentation | **[� API Docs](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/api-reference)** |
| 💻 **Examples** | Working Arduino code | **[📋 Examples](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/examples)** |
| ⚙️ **Configuration** | Device setup guide | **[🔧 Config](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/configuration)** |
| 🆘 **Troubleshooting** | Solutions to common issues | **[🛠️ Help](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/troubleshooting)** |

---

### 🎯 LoRa-Specific Documentation

#### Wake-on-Radio (WOR) Power Management
Maximize battery life in your LoRa projects with Wake-on-Radio mode:

- **[📡 WOR Documentation](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/wor)** - Complete guide to power-saving techniques
- **[💤 WOR Example](examples/06_sendWORMessage/)** - Send messages to sleeping devices
- **[⏰ Wake from WOR](examples/06_wakeUPLoRaFromWOR/)** - Receive and handle WOR messages
- **Power Savings**: Reduce power consumption from 100mA to <10μA in sleep mode
- **Use Cases**: Battery-powered sensors, remote monitoring, solar-powered nodes

#### Range Optimization Strategies
Achieve maximum communication range (5-10km) with proper configuration:

- **[📏 Range Optimization Guide](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/configuration)** - Maximize transmission distance
- **[🔧 Configuration Guide](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/configuration)** - Optimize power, speed, and channel settings
- **[📶 Wiring Guide](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/wiring)** - Proper antenna connections
- **Key Factors:**
  - **Transmission Power**: Up to 30dBm (1W) configurable
  - **Air Data Rate**: Lower rates = longer range (1.2kbps to 62.5kbps)
  - **Antenna Quality**: Use proper 50Ω antennas matched to frequency
  - **Line of Sight**: Clear path significantly improves range
  - **Frequency Band**: Choose appropriate band (433MHz, 868MHz, 915MHz)

#### Communication Modes
Choose the right mode for your application:

- **[🔄 Transparent Mode](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/transparent-mode)** - Simple peer-to-peer communication
  - No address configuration needed
  - Broadcast to all devices on same channel
  - Best for: Simple point-to-point links

- **[🎯 Fixed Mode](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/fixed-mode)** - Addressed messaging with channel control
  - Target specific devices by address
  - Control channel and power per message
  - Best for: Complex networks, multiple nodes

- **[📢 Broadcast Mode](examples/07_sendBroadcastMessage/)** - One-to-many communication
  - Send to all devices simultaneously
  - Efficient for sensor networks
  - Best for: Environmental monitoring, alerts

#### Multi-Platform Support
Platform-specific guides for seamless integration:

- **[🔷 Arduino Setup](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/platform-arduino)** - Arduino UNO, Nano, Mega
- **[🔶 ESP32 Setup](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/platform-esp32)** - ESP32 with hardware serial
- **[🔸 ESP8266 Setup](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/platform-esp8266)** - ESP8266 with software serial
- **[📋 All Platforms](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/platform-setup)** - Complete platform guide (STM32, Pi Pico, SAMD)

#### LLCC68 Chipset Features
- **Frequency Bands:** 433MHz, 868MHz, 915MHz (region-specific)
- **Range:** 5-10km line-of-sight, 1-2km urban environments
- **Data Rates:** 1.2kbps to 62.5kbps (configurable)
- **Power Output:** Up to 30dBm (1W) maximum
- **Interface:** UART (3.3V/5V compatible with level shifter)
- **Current:** 100mA TX, 15mA RX, <10μA sleep mode

---

### 📚 Documentation Highlights

- **[📖 Complete API Reference](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/api-reference)** - Every method documented with examples
- **[🚀 5-Minute Quick Start](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/quickstart)** - Get your first LoRa project working
- **[📚 Examples Library](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/examples)** - 8+ complete Arduino sketches  
- **[🔧 Platform Setup Guides](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/platform-setup)** - Arduino, ESP32, ESP8266, STM32, Pi Pico
- **[⚙️ Configuration Guide](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/configuration)** - Optimize for your application
- **[🛠️ Troubleshooting](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/troubleshooting)** - Solutions to common issues
- **[📊 Performance Optimization](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/performance)** - Maximize range and efficiency
- **[🔒 Security Guide](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/encryption)** - Encryption and secure communication

### 🎯 Documentation Features

- **✅ 100% API Coverage**: Every method documented with examples
- **✅ Multi-Platform**: Arduino, ESP32, STM32, Pi Pico specific notes  
- **✅ Real Examples**: Working code for every major feature
- **✅ Error Handling**: Comprehensive status codes and recovery
- **✅ Performance Data**: Range testing and power consumption
- **✅ Auto-Generated**: Always up-to-date with latest code changes

### 🔄 Documentation Updates

Documentation is automatically updated on every code change:

[![Documentation](https://github.com/Alteriom/alteriom-ebyte-lora-e220-library/actions/workflows/documentation.yml/badge.svg)](https://github.com/Alteriom/alteriom-ebyte-lora-e220-library/actions/workflows/documentation.yml)

**View Latest**: [https://alteriom.github.io/alteriom-ebyte-lora-e220-library/](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/)

### 📑 Complete Documentation Index

For a comprehensive list of all documentation resources, including LoRa-specific guides, see:
**[📚 Documentation Index](.github/DOCUMENTATION.md)** - Complete documentation reference with categorized links

## 🤝 Contributing

We welcome contributions! Please see our [Contributing Guidelines](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/CONTRIBUTING) for details.

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

- **GitHub Repository**: https://github.com/Alteriom/alteriom-ebyte-lora-e220-library
- **Original Author's Website**: https://www.mischianti.org
- **Arduino Library Registry**: [Submit here](https://github.com/arduino/library-registry)
- **PlatformIO Registry**: [View package](https://registry.platformio.org/libraries/alteriom/Alteriom_EByte_LoRa_E220)

---

**Ready to get started?** Check out our [Quick Start Guide](https://alteriom.github.io/alteriom-ebyte-lora-e220-library/#/quickstart) and start building your LoRa projects today!
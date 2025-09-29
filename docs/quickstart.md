# 🚀 Quick Start Guide

Get up and running with the Alteriom EByte LoRa E220 library in under 5 minutes!

## 📦 Installation

<!-- tabs:start -->

#### **Arduino IDE**

1. Open Arduino IDE
2. Go to **Tools** → **Manage Libraries**
3. Search for "**Alteriom_EByte_LoRa_E220**"
4. Click **Install**

#### **PlatformIO**

Add to your `platformio.ini`:

```ini
lib_deps = 
    Alteriom/Alteriom_EByte_LoRa_E220@^1.1.6
```

#### **Manual Download**

1. Download from [GitHub Releases](https://github.com/Alteriom/EByte_LoRa_E220_Series_Library/releases)
2. Extract to your Arduino libraries folder
3. Restart Arduino IDE

<!-- tabs:end -->

## 🔌 Basic Wiring

### Arduino Uno/Nano

| E220 Pin | Arduino Pin | Description |
|----------|-------------|-------------|
| VCC      | 3.3V        | Power supply |
| GND      | GND         | Ground |
| RXD      | Pin 2       | Software Serial TX |
| TXD      | Pin 3       | Software Serial RX |
| AUX      | Pin 4       | Status indicator (optional) |
| M0       | Pin 5       | Mode control (optional) |
| M1       | Pin 6       | Mode control (optional) |

### ESP32

| E220 Pin | ESP32 Pin | Description |
|----------|-----------|-------------|
| VCC      | 3.3V      | Power supply |
| GND      | GND       | Ground |
| RXD      | GPIO 17   | Hardware Serial TX |
| TXD      | GPIO 16   | Hardware Serial RX |
| AUX      | GPIO 4    | Status indicator (optional) |
| M0       | GPIO 5    | Mode control (optional) |
| M1       | GPIO 18   | Mode control (optional) |

> ⚠️ **Important**: The E220 operates at 3.3V. If using 5V boards, use a voltage divider or level shifter.

## 💻 Your First Program

Here's a complete example to get you started:

```arduino
#include "Alteriom_EByte_LoRa_E220.h"

// For Arduino Uno/Nano (Software Serial)
LoRa_E220 e220ttl(2, 3, 4, 5, 6); // RX, TX, AUX, M0, M1

// For ESP32 (Hardware Serial)
// LoRa_E220 e220ttl(&Serial2, 4, 5, 18); // Serial, AUX, M0, M1

void setup() {
    Serial.begin(9600);
    delay(500);
    
    // Initialize the LoRa module
    if (e220ttl.begin()) {
        Serial.println("✅ LoRa E220 initialized successfully!");
    } else {
        Serial.println("❌ LoRa E220 initialization failed!");
        return;
    }
    
    // Get and print current configuration
    ResponseStructContainer response = e220ttl.getConfiguration();
    if (response.status.code == E220_SUCCESS) {
        Configuration* config = (Configuration*)response.data;
        Serial.println("📋 Current Configuration:");
        Serial.print("   Address: "); 
        Serial.print(config->ADDH); Serial.print("."); Serial.println(config->ADDL);
        Serial.print("   Channel: "); Serial.println(config->CHAN);
        Serial.print("   Speed: "); Serial.println(config->SPED.uartBaudRate);
    } else {
        Serial.println("❌ Failed to read configuration");
    }
    response.close(); // Always free memory!
    
    Serial.println("🚀 Ready to communicate!");
}

void loop() {
    // Send a message every 5 seconds
    static unsigned long lastSend = 0;
    if (millis() - lastSend > 5000) {
        lastSend = millis();
        
        String message = "Hello from Alteriom LoRa! Time: " + String(millis());
        ResponseStatus status = e220ttl.sendMessage(message);
        
        if (status.code == E220_SUCCESS) {
            Serial.println("📤 Message sent: " + message);
        } else {
            Serial.println("❌ Send failed: " + status.getResponseDescription());
        }
    }
    
    // Check for incoming messages
    if (e220ttl.available()) {
        ResponseContainer response = e220ttl.receiveMessage();
        if (response.status.code == E220_SUCCESS) {
            Serial.println("📥 Received: " + response.data);
        } else {
            Serial.println("❌ Receive failed: " + response.status.getResponseDescription());
        }
    }
    
    delay(100);
}
```

## 🎯 What This Example Does

1. **Initializes** the LoRa module with proper pins
2. **Reads configuration** to verify communication
3. **Sends messages** every 5 seconds with timestamp
4. **Listens** for incoming messages continuously
5. **Handles errors** with clear status messages

## 🔄 Testing Communication

To test communication between two devices:

1. **Upload the same code** to two devices
2. **Power both devices** and open serial monitors
3. **Watch for messages** appearing on both serial monitors
4. **Check signal strength** if messages aren't received

## ⚡ Next Steps

Now that you have basic communication working:

- 📖 [Explore more examples](examples.md)
- 🔧 [Configure advanced parameters](configuration.md)
- 📡 [Learn about transmission modes](transparent-mode.md)
- 🛠️ [Set up your specific platform](platform-setup.md)
- 🎯 [Optimize performance](performance.md)

## 🆘 Need Help?

- 📚 Check the [FAQ](faq.md)
- 🔍 Browse [troubleshooting guide](troubleshooting.md)
- 💬 [Ask questions on GitHub](https://github.com/Alteriom/EByte_LoRa_E220_Series_Library/issues)
- 📧 [Contact us](mailto:contact@alteriom.com)

---

🎉 **Congratulations!** You now have LoRa communication working with the Alteriom EByte E220 library!
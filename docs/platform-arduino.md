# Arduino Platform Setup

## Overview

Arduino is the most common platform for E220 development, supporting Uno, Nano, Mega, and compatible boards. Since most Arduino boards operate at 5V while the E220 requires 3.3V, proper voltage handling is critical.

## Supported Arduino Boards

- ✅ **Arduino Uno** (ATmega328P)
- ✅ **Arduino Nano** (ATmega328P) 
- ✅ **Arduino Mega** (ATmega2560)
- ✅ **Arduino Leonardo** (ATmega32u4)
- ✅ **Arduino Pro Mini** (3.3V version recommended)

## Hardware Requirements

### Voltage Considerations
⚠️ **Critical**: Arduino operates at 5V, E220 requires 3.3V

**Power Options**:
1. **External 3.3V regulator** (recommended)
2. **Arduino 3.3V pin** (limited to 50mA)
3. **Voltage dividers** for signal levels

### Pin Requirements
- 2 pins for UART (software serial)
- 3 pins for control (M0, M1, AUX)
- Total: 5 digital pins minimum

## Basic Wiring

### Arduino Uno with Voltage Dividers
```
E220 Module     Arduino Uno
-----------     -----------
VCC       ←─────── 3.3V (limited current)
GND       ←─────── GND
TX        ←─────── Pin 2 (SoftwareSerial RX)
RX        ←─────── Pin 3 → [1kΩ] → [2kΩ to GND]
M0        ←─────── Pin 4 → [1kΩ] → [2kΩ to GND]  
M1        ←─────── Pin 5 → [1kΩ] → [2kΩ to GND]
AUX       ←─────── Pin 6 (direct connection)
```

### Arduino Mega (Multiple Serial Ports)
```cpp
// Mega has multiple hardware serial ports
E220 Module     Arduino Mega
-----------     ------------
VCC       ←─────── 3.3V
GND       ←─────── GND
TX        ←─────── Pin 19 (Serial1 RX)
RX        ←─────── Pin 18 (Serial1 TX) + voltage divider
M0        ←─────── Pin 22 + voltage divider
M1        ←─────── Pin 23 + voltage divider
AUX       ←─────── Pin 24
```

## Software Setup

### Library Installation
1. Open Arduino IDE
2. Go to **Sketch** → **Include Library** → **Manage Libraries**
3. Search for "EByte LoRa E220"
4. Install the library by Renzo Mischianti

### Basic Code Template
```cpp
#include "LoRa_E220.h"
#include <SoftwareSerial.h>

// Pin definitions
#define E220_RX 2
#define E220_TX 3
#define E220_M0 4
#define E220_M1 5
#define E220_AUX 6

// Create LoRa object
LoRa_E220 e220ttl(E220_RX, E220_TX, E220_M0, E220_M1, E220_AUX, UART_BPS_RATE_9600);

void setup() {
  Serial.begin(9600);
  delay(500);
  
  // Start E220
  e220ttl.begin();
  
  Serial.println("E220 Arduino Setup Complete");
  
  // Optional: read configuration
  printModuleInformation();
}

void loop() {
  // Your main code here
  if (e220ttl.available()) {
    String message = e220ttl.receiveMessage();
    Serial.println("Received: " + message);
  }
  
  // Send test message every 10 seconds
  static unsigned long lastSend = 0;
  if (millis() - lastSend > 10000) {
    e220ttl.sendMessage("Hello from Arduino!");
    lastSend = millis();
  }
}

void printModuleInformation() {
  ResponseStructContainer c = e220ttl.getConfiguration();
  if (c.status.code == 1) {
    Serial.println("✅ E220 Configuration:");
    printParameters(c.data);
  } else {
    Serial.println("❌ Failed to read configuration");
  }
  c.close();
}
```

## Power Supply Solutions

### Option 1: External 3.3V Regulator (Recommended)
```
5V → AMS1117-3.3 → 3.3V
     (or similar)
     
Components needed:
- AMS1117-3.3 regulator
- 10μF input capacitor
- 10μF output capacitor
```

### Option 2: Level Shifter Module
```cpp
// Use a 5V ↔ 3.3V level shifter module
// Available on Amazon/eBay for ~$2

Arduino 5V  ↔  Level Shifter  ↔  E220 3.3V
Pin 3       ↔  HV1 - LV1      ↔  RX
Pin 4       ↔  HV2 - LV2      ↔  M0
Pin 5       ↔  HV3 - LV3      ↔  M1
```

### Option 3: Voltage Divider Circuit
```cpp
// For each signal pin (RX, M0, M1):
Arduino Pin → 1kΩ → E220 Pin
                  ↓
                 2kΩ → GND

// This gives: 5V × (2kΩ/(1kΩ+2kΩ)) = 3.33V
```

## Arduino-Specific Examples

### Simple Sensor Node (Arduino Uno)
```cpp
#include "LoRa_E220.h"
#include <SoftwareSerial.h>

LoRa_E220 e220ttl(2, 3, 4, 5, 6, UART_BPS_RATE_9600);

void setup() {
  Serial.begin(9600);
  e220ttl.begin();
  
  Serial.println("Arduino Sensor Node Ready");
}

void loop() {
  // Read analog sensor (e.g., temperature sensor on A0)
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (5.0 / 1023.0);
  float temperature = (voltage - 0.5) * 100;  // LM35 sensor
  
  // Create message
  String message = "TEMP:" + String(temperature, 1);
  
  // Send via LoRa
  ResponseStatus rs = e220ttl.sendMessage(message);
  if (rs.code == 1) {
    Serial.println("✅ Sent: " + message);
  } else {
    Serial.println("❌ Send failed");
  }
  
  delay(30000);  // Send every 30 seconds
}
```

### Gateway with Serial Output (Arduino Mega)
```cpp
#include "LoRa_E220.h"

// Use hardware Serial1 on Mega
LoRa_E220 e220ttl(&Serial1, 22, 23, 24);

void setup() {
  Serial.begin(9600);    // USB serial for monitoring
  Serial1.begin(9600);   // E220 communication
  
  e220ttl.begin();
  Serial.println("Arduino Mega Gateway Ready");
}

void loop() {
  if (e220ttl.available()) {
    String received = e220ttl.receiveMessage();
    
    // Parse and format for serial output
    Serial.print("[");
    Serial.print(millis());
    Serial.print("] ");
    Serial.println(received);
    
    // Optional: send to other systems via Serial
    // or log to SD card, send to Ethernet, etc.
  }
}
```

### Bidirectional Communication
```cpp
void bidirectionalExample() {
  // Check for incoming LoRa messages
  if (e220ttl.available()) {
    String received = e220ttl.receiveMessage();
    Serial.println("LoRa: " + received);
    
    // Echo back with timestamp
    String echo = "ECHO:" + String(millis()) + ":" + received;
    e220ttl.sendMessage(echo);
  }
  
  // Check for serial commands
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    if (command.startsWith("SEND:")) {
      String message = command.substring(5);
      e220ttl.sendMessage(message);
      Serial.println("Sent: " + message);
    }
  }
}
```

## Memory Considerations

### Arduino Uno (2KB RAM)
```cpp
// Optimize for limited memory
void conserveMemory() {
  // Use F() macro for string constants
  Serial.println(F("This saves RAM"));
  
  // Avoid large String objects
  char buffer[50];  // Fixed size buffer
  
  // Close ResponseStructContainer promptly
  ResponseStructContainer c = e220ttl.getConfiguration();
  // Use c.data immediately
  c.close();  // Free memory
}
```

### String vs char arrays
```cpp
// Memory-efficient approach
void efficientStringHandling() {
  char message[50];
  
  // Build message in buffer
  float temp = 25.6;
  sprintf(message, "TEMP:%.1f", temp);
  
  // Send as char array
  e220ttl.sendMessage(message);
}
```

## Performance Optimization

### UART Speed Matching
```cpp
void optimizeUARTSpeed() {
  // Match software serial speed to E220 configuration
  
  // For short range, high data rate:
  LoRa_E220 e220ttl(2, 3, 4, 5, 6, UART_BPS_RATE_115200);
  
  // Configure E220 for high speed air data rate
  ResponseStructContainer c = e220ttl.getConfiguration();
  Configuration config = c.data;
  config.SPED.airDataRate = AIR_DATA_RATE_62_5;  // 62.5 kbps
  e220ttl.setConfiguration(config, WRITE_CFG_PWR_DWN_SAVE);
  c.close();
}
```

### Interrupt-Driven Reception
```cpp
volatile bool messageReceived = false;

void setup() {
  // Attach interrupt to AUX pin
  attachInterrupt(digitalPinToInterrupt(6), auxInterrupt, FALLING);
}

void auxInterrupt() {
  messageReceived = true;
}

void loop() {
  if (messageReceived) {
    messageReceived = false;
    
    if (e220ttl.available()) {
      String message = e220ttl.receiveMessage();
      processMessage(message);
    }
  }
}
```

## Common Arduino Issues

### SoftwareSerial Limitations
```
Problem: SoftwareSerial conflicts with other libraries
Solution: Use hardware serial on Mega, or AltSoftSerial

Problem: SoftwareSerial unreliable above 57600 baud
Solution: Use lower baud rates or hardware serial

Problem: SoftwareSerial blocks interrupts
Solution: Use interrupt-driven approach with AUX pin
```

### Memory Issues
```
Problem: String objects cause memory fragmentation
Solution: Use char arrays and sprintf()

Problem: ResponseStructContainer not closed
Solution: Always call .close() after use

Problem: Stack overflow with large local variables
Solution: Use static or global variables for large data
```

### Power Issues
```
Problem: E220 not responding after power-on
Solution: Add delay(500) after power-on

Problem: Brown-out during transmission
Solution: Use adequate power supply (≥500mA capability)

Problem: Voltage drops during high power transmission
Solution: Add large capacitors (1000μF) near E220
```

## Testing and Debugging

### Basic Connectivity Test
```cpp
void testConnectivity() {
  Serial.println(F("Testing E220 connectivity..."));
  
  ResponseStructContainer c = e220ttl.getConfiguration();
  if (c.status.code == 1) {
    Serial.println(F("✅ E220 responding"));
    Serial.print(F("Address: "));
    Serial.println(c.data.ADDH * 256 + c.data.ADDL);
    Serial.print(F("Channel: "));
    Serial.println(c.data.CHAN);
  } else {
    Serial.println(F("❌ E220 not responding"));
    Serial.println(c.status.getResponseDescription());
  }
  c.close();
}
```

### Signal Quality Monitor
```cpp
void monitorSignal() {
  ResponseStructContainer c = e220ttl.getRSSI();
  if (c.status.code == 1) {
    int rssi = c.data.RSSIAmbientNoise;
    Serial.print(F("RSSI: "));
    Serial.print(rssi);
    Serial.println(F(" dBm"));
    
    if (rssi > -50) Serial.println(F("📶 Excellent"));
    else if (rssi > -70) Serial.println(F("📶 Good"));
    else if (rssi > -90) Serial.println(F("📶 Fair"));
    else Serial.println(F("📶 Poor"));
  }
  c.close();
}
```

## Project Examples

### Weather Station Data Logger
```cpp
#include "LoRa_E220.h"
#include <SoftwareSerial.h>

LoRa_E220 e220ttl(2, 3, 4, 5, 6, UART_BPS_RATE_9600);

struct WeatherData {
  float temperature;
  float humidity;
  float pressure;
  uint32_t timestamp;
};

void setup() {
  Serial.begin(9600);
  e220ttl.begin();
  Serial.println(F("Weather Station Ready"));
}

void loop() {
  WeatherData data = {
    .temperature = readTemperature(),
    .humidity = readHumidity(), 
    .pressure = readPressure(),
    .timestamp = millis()
  };
  
  // Send structured data
  ResponseStatus rs = e220ttl.sendFixedMessage(0x01, 0x00, 23, &data, sizeof(data));
  
  if (rs.code == 1) {
    Serial.println(F("Weather data sent"));
  }
  
  delay(300000);  // 5 minutes
}
```

## Best Practices for Arduino

### Code Organization
- Use F() macro for string constants to save RAM
- Close ResponseStructContainer objects promptly
- Use char arrays instead of String objects when possible
- Implement proper error handling

### Hardware Design
- Always use 3.3V power for E220
- Add decoupling capacitors (100μF + 10μF)
- Use level shifters or voltage dividers for 5V boards
- Plan for adequate current supply (≥500mA)

### Power Management
- Use Arduino sleep modes for battery applications
- Monitor AUX pin for transmission completion
- Implement watchdog timer for reliability
- Consider external RTC for accurate timing

## Related Documentation

- [Wiring Guide](wiring.md) - Physical connections
- [Examples Library](examples.md) - Complete code examples
- [Troubleshooting](troubleshooting.md) - Common issues
- [API Reference](api-reference.md) - Function documentation
# 💻 Examples Library

Complete collection of working examples for the Alteriom EByte LoRa E220 Series Library.

## 🚀 Getting Started Examples

### 01_getConfiguration

**Purpose**: Read and display the current device configuration.

**Location**: `examples/01_getConfiguration/01_getConfiguration.ino`

**What it demonstrates**:
- Device initialization
- Configuration reading
- Status handling
- Memory management

**Key code**:
```cpp
#include "Alteriom_EByte_LoRa_E220.h"

LoRa_E220 e220ttl(2, 3); // RX, TX pins

void setup() {
    Serial.begin(9600);
    e220ttl.begin();
    
    ResponseStructContainer response = e220ttl.getConfiguration();
    if (response.status.code == E220_SUCCESS) {
        Configuration* config = (Configuration*)response.data;
        
        Serial.println("Current Configuration:");
        Serial.print("Address: ");
        Serial.print(config->ADDH, HEX);
        Serial.print(".");
        Serial.println(config->ADDL, HEX);
        Serial.print("Channel: ");
        Serial.println(config->CHAN);
    }
    response.close(); // Important: Free memory!
}
```

### 01_setConfiguration

**Purpose**: Modify device configuration parameters.

**Location**: `examples/01_setConfiguration/01_setConfiguration.ino`

**What it demonstrates**:
- Configuration modification
- Parameter validation
- Persistent vs temporary settings
- Error handling

**Key concepts**:
- Address configuration (ADDH, ADDL)
- Channel selection (0-83)
- Speed settings (UART + Air data rate)
- Option flags (power, FEC, etc.)

## 📡 Communication Examples

### 02_sendTransparentTransmission

**Purpose**: Simple point-to-point communication in transparent mode.

**Location**: `examples/02_sendTransparentTransmission/02_sendTransparentTransmission.ino`

**What it demonstrates**:
- Transparent mode communication
- Message sending
- Basic error handling
- String message transmission

**Use case**: Simple peer-to-peer communication where all devices on the same channel receive messages.

### 04_sendFixedTransmission

**Purpose**: Targeted communication to specific devices.

**Location**: `examples/04_sendFixedTransmission/04_sendFixedTransmission.ino`

**What it demonstrates**:
- Fixed transmission mode
- Address-specific messaging
- Channel-specific targeting
- Network topology management

**Key features**:
```cpp
// Send to specific device
ResponseStatus rs = e220ttl.sendFixedMessage(
    0x00, 0x02,    // Target address: 0x0002
    15,            // Channel 15
    "Hello specific device!"
);
```

### 05_sendFixedTransmissionStructure

**Purpose**: Sending structured data (structs) between devices.

**Location**: `examples/05_sendFixedTransmissionStructure/05_sendFixedTransmissionStructure.ino`

**What it demonstrates**:
- Binary data transmission
- Struct serialization
- Data integrity
- Efficient bandwidth usage

**Example structure**:
```cpp
struct SensorData {
    float temperature;
    float humidity;
    uint16_t battery;
    uint32_t timestamp;
};
```

### 05_sendFixedTransmissionStructureReadPartial

**Purpose**: Reading partial data from structured transmissions.

**Location**: `examples/05_sendFixedTransmissionStructureReadPartial/`

**What it demonstrates**:
- Partial data reading
- Buffer management
- Data validation
- Error recovery

## 🔋 Power Management Examples

### 06_sendWORMessage

**Purpose**: Wake-on-Radio (WOR) message transmission for low-power applications.

**Location**: `examples/06_sendWORMessage/06_sendWORMessage.ino`

**What it demonstrates**:
- WOR transmission setup
- Power-efficient communication
- Sleep/wake cycles
- Battery optimization

**Power benefits**:
- Transmitter: Normal power consumption
- Receiver: Ultra-low power until wake signal
- Ideal for battery-powered sensors

### 06_wakeUPLoRaFromWOR

**Purpose**: Waking up devices from WOR sleep mode.

**Location**: `examples/06_wakeUPLoRaFromWOR/06_wakeUPLoRaFromWOR.ino`

**What it demonstrates**:
- WOR wake-up process
- Device state management
- Power mode transitions
- Response timing

## 📨 Advanced Communication Examples

### 07_receiveMessages

**Purpose**: Comprehensive message reception handling.

**Location**: `examples/07_receiveMessages/07_receiveMessages.ino`

**What it demonstrates**:
- Message reception
- Data validation
- Error handling
- Buffer management
- RSSI monitoring

**Key features**:
```cpp
void loop() {
    if (e220ttl.available()) {
        ResponseContainer response = e220ttl.receiveMessage();
        if (response.status.code == E220_SUCCESS) {
            Serial.println("Received: " + response.data);
            Serial.println("RSSI: " + String(e220ttl.getRSSI()));
        }
    }
}
```

### 07_sendBroadcastMessage

**Purpose**: Broadcasting messages to all devices on a channel.

**Location**: `examples/07_sendBroadcastMessage/07_sendBroadcastMessage.ino`

**What it demonstrates**:
- Broadcast communication
- Channel-wide messaging
- Network announcements
- Group communication

## 🛠️ Platform-Specific Examples

### Arduino Uno/Nano Examples

**Pin configuration**:
```cpp
LoRa_E220 e220ttl(2, 3, 4, 5, 6); // RX, TX, AUX, M0, M1
```

**Considerations**:
- Software Serial limitations
- Memory constraints
- 5V to 3.3V level shifting

### ESP32 Examples

**Pin configuration**:
```cpp
LoRa_E220 e220ttl(&Serial2, 4, 5, 18); // Serial, AUX, M0, M1
```

**Features**:
- Hardware Serial support
- Higher baud rates
- More memory available
- WiFi + LoRa combinations

### ESP8266 Examples

**Pin configuration**:
```cpp
LoRa_E220 e220ttl(4, 5, 12, 13, 14); // RX, TX, AUX, M0, M1
```

**Considerations**:
- Limited pins available
- 3.3V native operation
- WiFi power management

## 📊 Example Categories

| Category | Examples | Description |
|----------|----------|-------------|
| **Setup** | 01_getConfiguration<br>01_setConfiguration | Device initialization and configuration |
| **Basic Comm** | 02_sendTransparentTransmission | Simple communication patterns |
| **Targeted** | 04_sendFixedTransmission<br>05_sendFixedTransmissionStructure | Address-specific messaging |
| **Power** | 06_sendWORMessage<br>06_wakeUPLoRaFromWOR | Low-power applications |
| **Advanced** | 07_receiveMessages<br>07_sendBroadcastMessage | Complex communication scenarios |

## 🏃‍♂️ Quick Start Template

Use this template to start your own LoRa project:

```cpp
#include "Alteriom_EByte_LoRa_E220.h"

// Pin configuration (adjust for your board)
LoRa_E220 e220ttl(2, 3); // Arduino Uno: RX=2, TX=3
// LoRa_E220 e220ttl(&Serial2, 4, 5, 18); // ESP32: Serial2, AUX=4, M0=5, M1=18

void setup() {
    Serial.begin(9600);
    
    // Initialize LoRa
    if (!e220ttl.begin()) {
        Serial.println("❌ LoRa initialization failed!");
        while(1); // Stop execution
    }
    
    Serial.println("✅ LoRa initialized successfully!");
    
    // Optional: Configure device
    ResponseStructContainer configResponse = e220ttl.getConfiguration();
    if (configResponse.status.code == E220_SUCCESS) {
        Configuration* config = (Configuration*)configResponse.data;
        
        // Modify configuration if needed
        // config->CHAN = 23;
        // e220ttl.setConfiguration(*config);
        
        Serial.println("📋 Configuration loaded");
    }
    configResponse.close();
}

void loop() {
    // Send message every 5 seconds
    static unsigned long lastSend = 0;
    if (millis() - lastSend > 5000) {
        lastSend = millis();
        
        String message = "Hello from device! Time: " + String(millis());
        ResponseStatus status = e220ttl.sendMessage(message);
        
        if (status.code == E220_SUCCESS) {
            Serial.println("📤 Sent: " + message);
        } else {
            Serial.println("❌ Send failed: " + status.getResponseDescription());
        }
    }
    
    // Check for incoming messages
    if (e220ttl.available()) {
        ResponseContainer response = e220ttl.receiveMessage();
        if (response.status.code == E220_SUCCESS) {
            Serial.println("📥 Received: " + response.data);
        }
    }
    
    delay(100);
}
```

## 🔧 Customizing Examples

### Changing Pin Configuration

Update the constructor based on your hardware:

```cpp
// Software Serial (Arduino Uno/Nano)
LoRa_E220 e220ttl(2, 3);                    // Basic: RX, TX only
LoRa_E220 e220ttl(2, 3, 4, 5, 6);          // Full: RX, TX, AUX, M0, M1

// Hardware Serial (ESP32)
LoRa_E220 e220ttl(&Serial2);                // Basic: Serial only
LoRa_E220 e220ttl(&Serial2, 4, 5, 18);     // Full: Serial, AUX, M0, M1
```

### Adjusting Configuration

Modify device parameters for your application:

```cpp
ResponseStructContainer response = e220ttl.getConfiguration();
Configuration* config = (Configuration*)response.data;

config->ADDH = 0x00;           // Address high byte
config->ADDL = 0x01;           // Address low byte  
config->CHAN = 23;             // Channel (0-83)
config->SPED = 0x34;           // 9600 baud, 2.4k air rate
config->OPTION = 0x40;         // Default options

e220ttl.setConfiguration(*config);
response.close();
```

## 🆘 Troubleshooting Examples

If examples don't work as expected:

1. **Check wiring** - Verify pin connections match your constructor
2. **Power supply** - Ensure stable 3.3V supply to E220
3. **Serial monitor** - Set correct baud rate (usually 9600)
4. **Configuration** - Verify both devices have compatible settings
5. **Range** - Start with devices close together

## 🔗 Related Documentation

- [API Reference](api-reference.md) - Complete method documentation
- [Configuration Guide](configuration.md) - Parameter explanations
- [Platform Setup](platform-setup.md) - Hardware-specific setup
- [Troubleshooting](troubleshooting.md) - Common issues and solutions
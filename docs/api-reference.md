# 📚 API Reference

Complete reference for the Alteriom EByte LoRa E220 Series Library.

## 🏗️ Core Classes

### LoRa_E220

Main class for controlling the EByte E220 LoRa device.

#### Constructors

```cpp
// Software Serial (Arduino Uno/Nano)
LoRa_E220(byte rxPin, byte txPin);
LoRa_E220(byte rxPin, byte txPin, byte auxPin, byte m0Pin, byte m1Pin);

// Hardware Serial (ESP32/ESP8266)
LoRa_E220(HardwareSerial* serial);
LoRa_E220(HardwareSerial* serial, byte auxPin, byte m0Pin, byte m1Pin);

// SoftwareSerial object
LoRa_E220(SoftwareSerial* serial);
LoRa_E220(SoftwareSerial* serial, byte auxPin, byte m0Pin, byte m1Pin);
```

#### Core Methods

##### begin()
Initialize the LoRa module.

```cpp
bool begin();
```

**Returns**: `true` if initialization successful, `false` otherwise.

**Example**:
```cpp
LoRa_E220 e220ttl(2, 3);
if (e220ttl.begin()) {
    Serial.println("LoRa initialized successfully!");
}
```

##### getConfiguration()
Read the current device configuration.

```cpp
ResponseStructContainer getConfiguration();
```

**Returns**: `ResponseStructContainer` containing `Configuration` struct.

**Example**:
```cpp
ResponseStructContainer response = e220ttl.getConfiguration();
if (response.status.code == E220_SUCCESS) {
    Configuration* config = (Configuration*)response.data;
    Serial.println("Address: " + String(config->ADDH) + "." + String(config->ADDL));
    Serial.println("Channel: " + String(config->CHAN));
}
response.close(); // Always free memory!
```

##### setConfiguration()
Write configuration to the device.

```cpp
ResponseStatus setConfiguration(Configuration configuration, ProgramCommand saveType = WRITE_CFG_PWR_DWN_SAVE);
```

**Parameters**:
- `configuration`: Configuration struct with new settings
- `saveType`: Save mode (temporary or permanent)

**Returns**: `ResponseStatus` with operation result.

##### sendMessage()
Send a message in transparent mode.

```cpp
ResponseStatus sendMessage(String message);
ResponseStatus sendMessage(const char* message);
ResponseStatus sendMessage(void* message, byte size);
```

**Parameters**:
- `message`: Message content (string or byte array)
- `size`: Size of byte array message

**Returns**: `ResponseStatus` with operation result.

**Example**:
```cpp
ResponseStatus rs = e220ttl.sendMessage("Hello LoRa!");
if (rs.code == E220_SUCCESS) {
    Serial.println("Message sent successfully!");
} else {
    Serial.println("Send failed: " + rs.getResponseDescription());
}
```

##### sendFixedMessage()
Send a message to a specific address and channel.

```cpp
ResponseStatus sendFixedMessage(byte ADDH, byte ADDL, byte CHAN, String message);
ResponseStatus sendFixedMessage(byte ADDH, byte ADDL, byte CHAN, const char* message);
ResponseStatus sendFixedMessage(byte ADDH, byte ADDL, byte CHAN, void* message, byte size);
```

**Parameters**:
- `ADDH`: High byte of target address
- `ADDL`: Low byte of target address  
- `CHAN`: Target channel
- `message`: Message content
- `size`: Size of byte array message

##### sendBroadcastFixedMessage()
Send a broadcast message to all devices on a channel.

```cpp
ResponseStatus sendBroadcastFixedMessage(byte CHAN, String message);
ResponseStatus sendBroadcastFixedMessage(byte CHAN, const char* message);
ResponseStatus sendBroadcastFixedMessage(byte CHAN, void* message, byte size);
```

##### receiveMessage()
Receive an incoming message.

```cpp
ResponseContainer receiveMessage();
ResponseContainer receiveMessage(byte size);
ResponseStructContainer receiveMessage(byte size);
```

**Returns**: `ResponseContainer` with received message data.

**Example**:
```cpp
if (e220ttl.available()) {
    ResponseContainer response = e220ttl.receiveMessage();
    if (response.status.code == E220_SUCCESS) {
        Serial.println("Received: " + response.data);
    }
}
```

##### available()
Check if data is available to read.

```cpp
int available();
```

**Returns**: Number of bytes available to read.

## 📊 Data Structures

### Configuration
Device configuration parameters.

```cpp
struct Configuration {
    byte ADDH;      // Address high byte
    byte ADDL;      // Address low byte
    byte SPED;      // Speed configuration
    byte CHAN;      // Channel (0-83)
    byte OPTION;    // Option settings
};
```

### ModuleInformation
Device identification and capabilities.

```cpp
struct ModuleInformation {
    byte features;
    byte frequency;
    byte version;
};
```

### ResponseStatus
Operation result with descriptive information.

```cpp
struct ResponseStatus {
    E220_RESPONSE_STATUS code;
    String getResponseDescription();
};
```

### ResponseContainer
Generic response wrapper for string data.

```cpp
struct ResponseContainer {
    String data;
    ResponseStatus status;
};
```

### ResponseStructContainer
Generic response wrapper for struct data.

```cpp
struct ResponseStructContainer {
    void* data;
    ResponseStatus status;
    void close(); // Free allocated memory
};
```

## 🔧 Constants and Enums

### Response Codes

```cpp
enum E220_RESPONSE_STATUS {
    E220_SUCCESS = 1,
    ERR_E220_UNKNOWN,
    ERR_E220_NOT_SUPPORT,
    ERR_E220_NOT_IMPLEMENT,
    ERR_E220_NOT_INITIAL,
    ERR_E220_INVALID_PARAM,
    ERR_E220_DATA_SIZE_NOT_MATCH,
    ERR_E220_BUF_TOO_SMALL,
    ERR_E220_TIMEOUT,
    ERR_E220_HARDWARE,
    ERR_E220_HEAD_NOT_RECOGNIZED,
    ERR_E220_NO_RESPONSE_FROM_DEVICE,
    ERR_E220_WRONG_UART_CONFIG,
    ERR_E220_PACKET_TOO_BIG
};
```

### Program Commands

```cpp
enum ProgramCommand {
    WRITE_CFG_PWR_DWN_SAVE = 0xC0,
    READ_CFG = 0xC1,
    WRITE_CFG_PWR_DWN_LOSE = 0xC2
};
```

### UART Baud Rates

```cpp
enum UART_BPS_TYPE {
    UART_BPS_1200 = 0x00,
    UART_BPS_2400 = 0x01,
    UART_BPS_4800 = 0x02,
    UART_BPS_9600 = 0x03,
    UART_BPS_19200 = 0x04,
    UART_BPS_38400 = 0x05,
    UART_BPS_57600 = 0x06,
    UART_BPS_115200 = 0x07
};
```

### Air Data Rates

```cpp
enum AIR_DATA_RATE {
    AIR_DATA_RATE_000_24 = 0x00,
    AIR_DATA_RATE_001_24 = 0x01,
    AIR_DATA_RATE_010_24 = 0x02,
    AIR_DATA_RATE_011_48 = 0x03,
    AIR_DATA_RATE_100_48 = 0x04,
    AIR_DATA_RATE_101_96 = 0x05,
    AIR_DATA_RATE_110_192 = 0x06,
    AIR_DATA_RATE_111_384 = 0x07
};
```

## 💡 Usage Examples

### Basic Communication

```cpp
#include "Alteriom_EByte_LoRa_E220.h"

LoRa_E220 e220ttl(2, 3); // RX, TX pins

void setup() {
    Serial.begin(9600);
    e220ttl.begin();
}

void loop() {
    // Send message
    e220ttl.sendMessage("Hello World!");
    
    // Check for incoming messages
    if (e220ttl.available()) {
        ResponseContainer response = e220ttl.receiveMessage();
        Serial.println("Received: " + response.data);
    }
    
    delay(1000);
}
```

### Configuration Management

```cpp
// Read current configuration
ResponseStructContainer configResponse = e220ttl.getConfiguration();
Configuration* config = (Configuration*)configResponse.data;

// Modify configuration
config->CHAN = 23;  // Change to channel 23
config->ADDH = 0x00;
config->ADDL = 0x01;

// Write back to device
ResponseStatus status = e220ttl.setConfiguration(*config);
configResponse.close();
```

### Fixed Transmission

```cpp
// Send message to specific device
ResponseStatus rs = e220ttl.sendFixedMessage(
    0x00, 0x02,    // Target address: 0x0002
    15,            // Channel 15
    "Targeted message"
);
```

## 🆘 Error Handling

Always check return status and handle errors appropriately:

```cpp
ResponseStatus status = e220ttl.sendMessage("Test");
if (status.code != E220_SUCCESS) {
    Serial.println("Error: " + status.getResponseDescription());
    // Handle error condition
}
```

Remember to call `close()` on `ResponseStructContainer` objects to free memory:

```cpp
ResponseStructContainer response = e220ttl.getConfiguration();
// Use response.data...
response.close(); // Important: Free memory!
```

## 🔗 Related Documentation

- [Configuration Guide](configuration.md) - Detailed parameter explanations
- [Examples](examples.md) - Complete working examples
- [Troubleshooting](troubleshooting.md) - Common issues and solutions
- [Platform Setup](platform-setup.md) - Platform-specific configuration
# Fixed Mode

## Overview

Fixed transmission mode allows precise control over addressing and routing. Unlike transparent mode, fixed mode adds address headers to packets, enabling point-to-point, point-to-multipoint, and broadcast communications.

## How It Works

```
[Device A] → [ADDH:ADDL:CHAN] → [LoRa Network] → [Device B with matching address]
```

Each packet includes:
- **ADDH**: High address byte (0x00-0xFF)
- **ADDL**: Low address byte (0x00-0xFF) 
- **CHAN**: Channel number (0-80)

## Configuration

### Enable Fixed Mode
```cpp
ResponseStructContainer c = e220ttl.getConfiguration();
Configuration configuration = c.data;

// Enable fixed transmission
configuration.TRANSMISSION_MODE.fixedTransmission = FT_FIXED_TRANSMISSION;

e220ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
c.close();
```

### Basic Setup
```cpp
#include "LoRa_E220.h"

LoRa_E220 e220ttl(2, 3, 4, 5, 6, UART_BPS_RATE_9600);

void setup() {
  Serial.begin(9600);
  e220ttl.begin();
  
  // Configure device address
  ResponseStructContainer c = e220ttl.getConfiguration();
  Configuration configuration = c.data;
  
  configuration.ADDH = 0x00;  // High address
  configuration.ADDL = 0x01;  // Low address (Device 1)
  configuration.CHAN = 23;    // Channel 23
  configuration.TRANSMISSION_MODE.fixedTransmission = FT_FIXED_TRANSMISSION;
  
  e220ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
  c.close();
  
  Serial.println("Device configured for fixed mode");
}
```

## Addressing Scheme

### Address Format
- **16-bit addressing**: ADDH (8 bits) + ADDL (8 bits)
- **Total devices**: Up to 65,536 unique addresses
- **Channel separation**: 80 channels available

### Address Examples
```cpp
// Device addresses
#define SENSOR_NODE_1    0x0001  // ADDH=0x00, ADDL=0x01
#define SENSOR_NODE_2    0x0002  // ADDH=0x00, ADDL=0x02
#define GATEWAY          0x0100  // ADDH=0x01, ADDL=0x00
#define BROADCAST        0xFFFF  // ADDH=0xFF, ADDL=0xFF
```

## Communication Patterns

### Point-to-Point
```cpp
void sendToSpecificDevice() {
  String message = "Hello Device 2";
  
  // Send to device with address 0x0002 on channel 23
  ResponseStatus rs = e220ttl.sendFixedMessage(0x00, 0x02, 23, message);
  
  if (rs.code == 1) {
    Serial.println("✅ Message sent successfully");
  } else {
    Serial.println("❌ Send failed: " + rs.getResponseDescription());
  }
}
```

### Broadcast Message
```cpp
void sendBroadcast() {
  String announcement = "System maintenance at 2 PM";
  
  // Broadcast to all devices on channel 23
  ResponseStatus rs = e220ttl.sendBroadcastFixedMessage(23, announcement);
  
  Serial.println("📢 Broadcast sent");
}
```

### Multi-Channel Network
```cpp
void multiChannelSetup() {
  // Sensors on channel 23
  // Actuators on channel 24
  // Management on channel 25
  
  String sensorData = "temp:23.5,humidity:65";
  e220ttl.sendFixedMessage(0x01, 0x00, 23, sensorData);  // To gateway
}
```

## Advanced Features

### Message Structure
```cpp
struct SensorMessage {
  uint8_t sensorId;
  float temperature;
  float humidity;
  uint32_t timestamp;
};

void sendStructuredData() {
  SensorMessage data = {1, 23.5, 65.2, millis()};
  
  ResponseStatus rs = e220ttl.sendFixedMessage(
    0x01, 0x00, 23,  // To gateway on channel 23
    &data, sizeof(data)
  );
}
```

### Acknowledgment System
```cpp
bool sendWithRetry(uint8_t addh, uint8_t addl, uint8_t chan, String message) {
  const int MAX_RETRIES = 3;
  const unsigned long TIMEOUT = 2000;
  
  for (int retry = 0; retry < MAX_RETRIES; retry++) {
    // Send message
    e220ttl.sendFixedMessage(addh, addl, chan, message);
    
    // Wait for acknowledgment
    unsigned long startTime = millis();
    while (millis() - startTime < TIMEOUT) {
      if (e220ttl.available()) {
        String response = e220ttl.receiveMessage();
        if (response == "ACK") {
          return true;  // Success
        }
      }
    }
    
    Serial.println("Retry " + String(retry + 1));
  }
  
  return false;  // Failed after retries
}
```

### Message Routing
```cpp
void messageRouter() {
  if (e220ttl.available()) {
    ResponseContainer rc = e220ttl.receiveMessageComplete();
    
    uint8_t sourceAddH = rc.data.substring(0, 1).toInt();
    uint8_t sourceAddL = rc.data.substring(1, 2).toInt();
    String payload = rc.data.substring(3);
    
    Serial.println("From: " + String(sourceAddH) + ":" + String(sourceAddL));
    Serial.println("Message: " + payload);
    
    // Route based on content
    if (payload.startsWith("SENSOR:")) {
      processSensorData(payload);
    } else if (payload.startsWith("ALERT:")) {
      processAlert(payload);
    }
    
    rc.close();
  }
}
```

## Network Topologies

### Star Network (Hub and Spoke)
```cpp
// Gateway configuration
#define GATEWAY_ADDR_H   0x01
#define GATEWAY_ADDR_L   0x00

void gatewayLoop() {
  // Collect data from all sensors
  for (int sensor = 1; sensor <= 10; sensor++) {
    String request = "DATA_REQUEST";
    e220ttl.sendFixedMessage(0x00, sensor, 23, request);
    
    // Wait for response
    delay(100);
    if (e220ttl.available()) {
      String data = e220ttl.receiveMessage();
      processSensorData(sensor, data);
    }
  }
}
```

### Mesh Network (Relay)
```cpp
void relayMessage() {
  if (e220ttl.available()) {
    ResponseContainer rc = e220ttl.receiveMessageComplete();
    
    // Check if message is for this device
    if (isForMe(rc.data)) {
      processMessage(rc.data);
    } else {
      // Relay to next hop
      uint8_t nextHop = findNextHop(rc.destinationAddress);
      e220ttl.sendFixedMessage(getAddH(nextHop), getAddL(nextHop), 23, rc.data);
    }
    
    rc.close();
  }
}
```

## Power Optimization

### Sleep Mode with Addressing
```cpp
void lowPowerSensor() {
  // Send data
  String data = readSensorData();
  e220ttl.sendFixedMessage(0x01, 0x00, 23, data);
  
  // Enter sleep mode
  e220ttl.setMode(MODE_2_WOR_RECEIVER);
  
  // Sleep microcontroller
  delay(30000);  // Sleep 30 seconds
  
  // Wake up and repeat
  e220ttl.setMode(MODE_0_TRANSMISSION);
}
```

### Wake-on-Radio with Fixed Addressing
```cpp
void worWithAddressing() {
  ResponseStructContainer c = e220ttl.getConfiguration();
  Configuration configuration = c.data;
  
  // Configure WOR with fixed transmission
  configuration.TRANSMISSION_MODE.fixedTransmission = FT_FIXED_TRANSMISSION;
  configuration.TRANSMISSION_MODE.WORPeriod = WOR_2000_000;  // 2 second wake cycle
  
  e220ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
  c.close();
}
```

## Error Handling and Diagnostics

### Address Validation
```cpp
bool isValidAddress(uint8_t addh, uint8_t addl) {
  // Reserved addresses
  if (addh == 0xFF && addl == 0xFF) return false;  // Broadcast
  if (addh == 0x00 && addl == 0x00) return false;  // Default
  
  return true;
}
```

### Network Discovery
```cpp
void discoverNodes() {
  Serial.println("🔍 Discovering network nodes...");
  
  for (int addr = 1; addr <= 255; addr++) {
    String ping = "PING";
    e220ttl.sendFixedMessage(0x00, addr, 23, ping);
    
    delay(100);  // Wait for response
    
    if (e220ttl.available()) {
      String response = e220ttl.receiveMessage();
      if (response == "PONG") {
        Serial.println("📡 Found device at address: " + String(addr));
      }
    }
  }
}
```

### Signal Quality per Device
```cpp
void monitorDeviceSignals() {
  for (int device : knownDevices) {
    String request = "RSSI_REQUEST";
    e220ttl.sendFixedMessage(0x00, device, 23, request);
    
    if (e220ttl.available()) {
      String response = e220ttl.receiveMessageRSSI();
      Serial.println("Device " + String(device) + " RSSI: " + response);
    }
  }
}
```

## Performance Considerations

### Packet Overhead
- **Address bytes**: 3 bytes per packet (ADDH + ADDL + CHAN)
- **Maximum payload**: Reduced by addressing overhead
- **Efficiency**: ~97% for 200-byte packets

### Collision Avoidance
```cpp
void randomBackoff() {
  // Random delay before transmission
  int backoff = random(10, 100);
  delay(backoff);
}

void carrierSense() {
  // Check if channel is busy
  ResponseStructContainer c = e220ttl.getRSSI();
  if (c.data.RSSIAmbientNoise > -90) {
    delay(random(50, 200));  // Channel busy, wait
  }
  c.close();
}
```

## Example Applications

### Sensor Network Gateway
```cpp
void sensorGateway() {
  if (e220ttl.available()) {
    ResponseContainer rc = e220ttl.receiveMessageComplete();
    
    // Parse sensor data
    uint8_t sensorId = rc.data.substring(0, 2).toInt();
    float value = rc.data.substring(3).toFloat();
    
    // Log to database
    logSensorData(sensorId, value, millis());
    
    // Send acknowledgment
    e220ttl.sendFixedMessage(0x00, sensorId, 23, "ACK");
    
    rc.close();
  }
}
```

### Remote Control System
```cpp
void remoteController() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    
    // Parse command: "device:action"
    int colon = command.indexOf(':');
    int deviceId = command.substring(0, colon).toInt();
    String action = command.substring(colon + 1);
    
    // Send to specific device
    e220ttl.sendFixedMessage(0x00, deviceId, 24, action);
    
    Serial.println("Command sent to device " + String(deviceId));
  }
}
```

## Best Practices

### Address Management
- Use systematic addressing scheme
- Reserve ranges for device types
- Document address assignments
- Avoid address conflicts

### Network Design
- Plan channel allocation
- Consider interference patterns
- Implement collision detection
- Use appropriate power levels

### Message Design
- Include sequence numbers
- Add checksums for integrity
- Use structured data formats
- Keep messages concise

## Troubleshooting

### No Response from Specific Device
1. Verify device address matches
2. Check channel configuration
3. Test with broadcast message
4. Verify device is powered and in range

### Address Conflicts
1. Use network discovery to find conflicts
2. Implement dynamic addressing
3. Check configuration consistency
4. Monitor for duplicate responses

### Performance Issues
1. Reduce packet size and frequency
2. Implement proper backoff algorithms
3. Use multiple channels
4. Monitor collision rates

## Related Documentation

- [Transparent Mode](transparent-mode.md) - Simple point-to-point communication
- [Wake-on-Radio](wor.md) - Power-saving techniques
- [Configuration Guide](configuration.md) - Parameter details
- [API Reference](api-reference.md) - Function documentation
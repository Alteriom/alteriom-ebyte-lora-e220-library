# Transparent Mode

## Overview

Transparent transmission mode is the simplest way to use the E220 module. In this mode, any data sent to the module's UART is automatically transmitted via LoRa radio, and any received LoRa data is output through the UART.

## How It Works

```
Device A ←→ UART ←→ E220 ←── LoRa Radio ──→ E220 ←→ UART ←→ Device B
```

The E220 module acts as a "wireless UART cable" - completely transparent to your application.

## Configuration

### Mode Settings
```cpp
// Set transparent mode (M0=LOW, M1=LOW)
digitalWrite(M0_PIN, LOW);
digitalWrite(M1_PIN, LOW);
```

### Basic Setup
```cpp
#include "LoRa_E220.h"

LoRa_E220 e220ttl(2, 3, 4, 5, 6, UART_BPS_RATE_9600);

void setup() {
  Serial.begin(9600);
  e220ttl.begin();
  
  // The module is now in transparent mode by default
  Serial.println("E220 ready for transparent communication");
}

void loop() {
  // Any data sent here will be transmitted
  if (Serial.available()) {
    String message = Serial.readString();
    e220ttl.sendMessage(message);
  }
  
  // Check for received data
  if (e220ttl.available()) {
    String received = e220ttl.receiveMessage();
    Serial.println("Received: " + received);
  }
}
```

## Advantages

### Simplicity
- **No addressing**: Just send data, it gets transmitted
- **Automatic**: No manual packet formatting required
- **Real-time**: Low latency communication
- **Bidirectional**: Both devices can send/receive simultaneously

### Use Cases
- Point-to-point communication
- Serial data bridging
- Simple sensor networks
- Legacy system integration

## Limitations

### Range and Power
- **No routing**: Direct point-to-point only
- **Power usage**: Continuously powered (no sleep)
- **Collision**: Multiple transmitters can interfere

### Data Management
- **No acknowledgment**: No confirmation of delivery
- **No error detection**: Limited built-in error handling
- **Buffer limits**: UART buffer constraints

## Advanced Configuration

### Optimizing for Transparent Mode
```cpp
void configureForTransparent() {
  ResponseStructContainer c = e220ttl.getConfiguration();
  Configuration configuration = c.data;
  
  // Optimize settings
  configuration.TRANSMISSION_MODE.enableRSSI = RSSI_DISABLED;
  configuration.TRANSMISSION_MODE.fixedTransmission = FT_TRANSPARENT_TRANSMISSION;
  configuration.TRANSMISSION_MODE.WORPeriod = WOR_2000_000;
  
  e220ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
  c.close();
}
```

### Channel and Address Setup
```cpp
void setupChannel() {
  ResponseStructContainer c = e220ttl.getConfiguration();
  Configuration configuration = c.data;
  
  // Set same channel on all devices
  configuration.CHAN = 23;  // Channel 23 (433 + 23 = 456MHz)
  
  // Address not used in transparent mode, but can be set
  configuration.ADDH = 0x00;
  configuration.ADDL = 0x00;
  
  e220ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
  c.close();
}
```

## Performance Optimization

### UART Speed Matching
Match UART speed to air data rate:

```cpp
// High speed setup
LoRa_E220 e220ttl(2, 3, 4, 5, 6, UART_BPS_RATE_115200);

void setup() {
  e220ttl.begin();
  
  // Set high air data rate
  ResponseStructContainer c = e220ttl.getConfiguration();
  Configuration configuration = c.data;
  configuration.SPED.airDataRate = AIR_DATA_RATE_62_5;
  e220ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
  c.close();
}
```

### Buffer Management
```cpp
#define BUFFER_SIZE 200
char dataBuffer[BUFFER_SIZE];

void efficientReceive() {
  if (e220ttl.available()) {
    int bytesRead = 0;
    
    // Read all available data
    while (e220ttl.available() && bytesRead < BUFFER_SIZE - 1) {
      dataBuffer[bytesRead++] = e220ttl.read();
    }
    
    dataBuffer[bytesRead] = '\0';  // Null terminate
    Serial.println(dataBuffer);
  }
}
```

## Error Handling

### Connection Monitoring
```cpp
unsigned long lastReceived = 0;
const unsigned long TIMEOUT = 10000;  // 10 seconds

void checkConnection() {
  if (millis() - lastReceived > TIMEOUT) {
    Serial.println("⚠️ Connection timeout - check other device");
  }
}

void loop() {
  if (e220ttl.available()) {
    lastReceived = millis();
    // Process received data...
  }
  
  checkConnection();
}
```

### Signal Quality Monitoring
```cpp
void checkSignalQuality() {
  ResponseStructContainer c = e220ttl.getRSSI();
  if (c.status.code == 1) {
    int rssi = c.data.RSSIAmbientNoise;
    
    if (rssi > -50) {
      Serial.println("📶 Excellent signal");
    } else if (rssi > -70) {
      Serial.println("📶 Good signal");
    } else if (rssi > -90) {
      Serial.println("📶 Weak signal");
    } else {
      Serial.println("📶 Very weak signal");
    }
  }
  c.close();
}
```

## Example Applications

### Wireless Serial Bridge
```cpp
// Device A: Send sensor data
void sensorTransmitter() {
  float temperature = readTemperature();
  float humidity = readHumidity();
  
  String data = String(temperature) + "," + String(humidity);
  e220ttl.sendMessage(data);
  
  delay(5000);  // Send every 5 seconds
}

// Device B: Receive and display
void dataReceiver() {
  if (e220ttl.available()) {
    String received = e220ttl.receiveMessage();
    
    // Parse CSV data
    int comma = received.indexOf(',');
    float temp = received.substring(0, comma).toFloat();
    float hum = received.substring(comma + 1).toFloat();
    
    Serial.println("Temperature: " + String(temp) + "°C");
    Serial.println("Humidity: " + String(hum) + "%");
  }
}
```

### Bidirectional Chat
```cpp
void wirelessChat() {
  // Send message if user types
  if (Serial.available()) {
    String message = Serial.readStringUntil('\n');
    e220ttl.sendMessage("You: " + message);
    Serial.println("Sent: " + message);
  }
  
  // Display received messages
  if (e220ttl.available()) {
    String received = e220ttl.receiveMessage();
    Serial.println("Remote: " + received);
  }
}
```

## Best Practices

### Power Management
- Use WOR mode for battery applications
- Consider fixed mode for better power control
- Monitor AUX pin for transmission status

### Data Integrity
- Implement application-level checksums
- Use structured data formats (JSON, CSV)
- Add sequence numbers for ordering

### Network Design
- Use single channel for point-to-point
- Implement collision avoidance (random delays)
- Consider switching to fixed mode for multiple nodes

## Troubleshooting

### No Data Received
1. Check both devices on same channel
2. Verify UART connections and baud rates
3. Test with devices close together
4. Monitor RSSI for signal strength

### Garbled Data
1. Check UART baud rate matching
2. Verify air data rate settings
3. Check for electromagnetic interference
4. Test with shorter messages

### Poor Performance
1. Optimize UART vs air data rate ratio
2. Reduce transmission frequency
3. Implement proper buffering
4. Check antenna connections

## Related Documentation

- [Fixed Mode](fixed-mode.md) - For addressed communication
- [Wake-on-Radio](wor.md) - For power-saving applications
- [Configuration Guide](configuration.md) - Parameter details
- [API Reference](api-reference.md) - Function documentation
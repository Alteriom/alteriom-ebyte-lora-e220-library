# Wake-on-Radio (WOR)

## Overview

Wake-on-Radio (WOR) is a power-saving feature that allows the E220 module to enter deep sleep mode and wake up periodically to check for incoming messages. This dramatically reduces power consumption, making it ideal for battery-powered applications.

## How WOR Works

```
Normal Mode: [TX] ←→ [RX] ←→ [TX] ←→ [RX] (Continuous power consumption)

WOR Mode:    [Sleep] → [Wake] → [Listen] → [Sleep] → [Wake] → [Listen]
                        ↑         ↓
                     Low Power   Check for signals
```

The module wakes up at regular intervals, listens for a preamble signal, and either receives data or returns to sleep.

## WOR Configuration

### Enable WOR Mode
```cpp
ResponseStructContainer c = e220ttl.getConfiguration();
Configuration configuration = c.data;

// Set WOR transmitter mode (M0=LOW, M1=HIGH)
configuration.TRANSMISSION_MODE.WORPeriod = WOR_2000_000;  // 2 second wake cycle

e220ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
c.close();

// Set hardware pins for WOR transmitter
digitalWrite(M0_PIN, LOW);
digitalWrite(M1_PIN, HIGH);
```

### WOR Receiver Setup
```cpp
// Set WOR receiver mode (M0=HIGH, M1=LOW)
digitalWrite(M0_PIN, HIGH);
digitalWrite(M1_PIN, LOW);

ResponseStructContainer c = e220ttl.getConfiguration();
Configuration configuration = c.data;
configuration.TRANSMISSION_MODE.WORPeriod = WOR_2000_000;  // Match transmitter
e220ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
c.close();
```

## WOR Periods

### Available Wake Periods
```cpp
// Wake-up period options
WOR_500_000    // 500ms
WOR_1000_000   // 1 second
WOR_1500_000   // 1.5 seconds
WOR_2000_000   // 2 seconds (recommended)
WOR_2500_000   // 2.5 seconds
WOR_3000_000   // 3 seconds
WOR_3500_000   // 3.5 seconds
WOR_4000_000   // 4 seconds
```

### Choosing Wake Period
```cpp
void configureWORPeriod(uint8_t period) {
  ResponseStructContainer c = e220ttl.getConfiguration();
  Configuration configuration = c.data;
  
  switch (period) {
    case 1: configuration.TRANSMISSION_MODE.WORPeriod = WOR_500_000; break;
    case 2: configuration.TRANSMISSION_MODE.WORPeriod = WOR_1000_000; break;
    case 3: configuration.TRANSMISSION_MODE.WORPeriod = WOR_2000_000; break;  // Recommended
    case 4: configuration.TRANSMISSION_MODE.WORPeriod = WOR_4000_000; break;
  }
  
  e220ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
  c.close();
}
```

## Power Consumption

### Current Draw Comparison
```
Operating Mode     | Current Draw | Battery Life (2000mAh)
-------------------|--------------|----------------------
Transparent        | ~12mA        | ~7 days
Fixed Transmission | ~12mA        | ~7 days
WOR Receiver       | ~2μA         | ~45 years
WOR Transmitter    | ~2μA         | ~45 years (sleep)
```

### Battery Life Calculation
```cpp
float calculateBatteryLife(float batteryCapacity, float dutyCycle) {
  // batteryCapacity in mAh
  // dutyCycle: fraction of time active (0.0 to 1.0)
  
  float sleepCurrent = 0.002;    // 2μA
  float activeCurrent = 12.0;    // 12mA
  
  float avgCurrent = (dutyCycle * activeCurrent) + ((1 - dutyCycle) * sleepCurrent);
  float batteryLifeHours = batteryCapacity / avgCurrent;
  
  return batteryLifeHours / 24;  // Convert to days
}
```

## WOR Transmission

### Sending WOR Messages
```cpp
void sendWORMessage() {
  // WOR requires extended preamble for sleeping receivers
  String message = "Hello from WOR transmitter";
  
  // Send with extended preamble
  ResponseStatus rs = e220ttl.sendMessage(message);
  
  if (rs.code == 1) {
    Serial.println("✅ WOR message sent");
  } else {
    Serial.println("❌ WOR send failed");
  }
  
  // Return to sleep
  delay(100);  // Allow transmission to complete
}
```

### WOR with Fixed Addressing
```cpp
void sendWORFixed() {
  String data = "sensor:23.5";
  
  // Send to specific WOR receiver
  ResponseStatus rs = e220ttl.sendFixedMessage(0x00, 0x02, 23, data);
  
  Serial.println("WOR fixed message sent");
}
```

## WOR Reception

### Receiving WOR Messages
```cpp
void worReceiveLoop() {
  // Module automatically wakes and sleeps
  // Check for received data
  if (e220ttl.available()) {
    String message = e220ttl.receiveMessage();
    Serial.println("WOR received: " + message);
    
    // Process message quickly before next sleep cycle
    processMessage(message);
  }
  
  // Optional: add small delay for processing
  delay(10);
}
```

### WOR with Acknowledgment
```cpp
void worWithAck() {
  if (e220ttl.available()) {
    String message = e220ttl.receiveMessage();
    
    // Send acknowledgment
    String ack = "ACK:" + message.substring(0, 10);  // Echo first 10 chars
    e220ttl.sendMessage(ack);
    
    Serial.println("WOR message processed and ACK sent");
  }
}
```

## Advanced WOR Applications

### Sensor Network with WOR
```cpp
// Sensor node (WOR transmitter)
void sensorNodeWOR() {
  // Wake up, read sensor, transmit, sleep
  float temperature = readTemperature();
  float humidity = readHumidity();
  
  String data = String(temperature) + "," + String(humidity);
  
  // Send to gateway
  e220ttl.sendFixedMessage(0x01, 0x00, 23, data);
  
  // Enter deep sleep for 5 minutes
  ESP.deepSleep(5 * 60 * 1000000);  // ESP32/ESP8266
}

// Gateway (WOR receiver)
void gatewayWOR() {
  if (e220ttl.available()) {
    ResponseContainer rc = e220ttl.receiveMessageComplete();
    
    // Log sensor data with timestamp
    String timestamp = String(millis());
    String logEntry = timestamp + "," + rc.data;
    
    Serial.println("Sensor data: " + logEntry);
    // Store to SD card, send to cloud, etc.
    
    rc.close();
  }
}
```

### Bi-directional WOR Communication
```cpp
void bidirectionalWOR() {
  static unsigned long lastTransmit = 0;
  const unsigned long TRANSMIT_INTERVAL = 30000;  // 30 seconds
  
  // Receive mode (most of the time)
  if (e220ttl.available()) {
    String message = e220ttl.receiveMessage();
    processIncomingMessage(message);
  }
  
  // Periodic transmission
  if (millis() - lastTransmit > TRANSMIT_INTERVAL) {
    // Switch to transmit mode temporarily
    digitalWrite(M1_PIN, HIGH);  // WOR transmitter
    delay(10);
    
    String status = getDeviceStatus();
    e220ttl.sendMessage(status);
    
    // Return to receive mode
    digitalWrite(M1_PIN, LOW);   // WOR receiver
    lastTransmit = millis();
  }
}
```

### WOR Network Synchronization
```cpp
void worSyncNetwork() {
  // Master sends sync pulse every hour
  static unsigned long lastSync = 0;
  const unsigned long SYNC_INTERVAL = 3600000;  // 1 hour
  
  if (millis() - lastSync > SYNC_INTERVAL) {
    String syncMessage = "SYNC:" + String(millis());
    
    // Broadcast sync to all nodes
    e220ttl.sendBroadcastFixedMessage(23, syncMessage);
    
    lastSync = millis();
    Serial.println("📡 Network sync pulse sent");
  }
}
```

## WOR Timing Considerations

### Preamble Requirements
```cpp
void calculatePreamble() {
  // WOR requires extended preamble to wake sleeping receivers
  // Preamble time = WOR wake period + safety margin
  
  float worPeriod = 2.0;      // 2 second WOR period
  float safetyMargin = 0.5;   // 500ms safety
  float requiredPreamble = worPeriod + safetyMargin;
  
  Serial.println("Required preamble: " + String(requiredPreamble) + " seconds");
}
```

### Message Scheduling
```cpp
void scheduleWORMessages() {
  // Schedule messages to avoid collisions
  static unsigned long messageSlots[] = {5000, 15000, 25000, 35000};
  static int currentSlot = 0;
  
  if (millis() % messageSlots[currentSlot] < 100) {
    sendWORMessage();
    currentSlot = (currentSlot + 1) % 4;
  }
}
```

## Power Optimization Strategies

### Microcontroller Sleep Integration
```cpp
void deepSleepWithWOR() {
  // Configure E220 for WOR
  digitalWrite(M0_PIN, HIGH);  // WOR receiver
  digitalWrite(M1_PIN, LOW);
  
  // Put microcontroller to sleep
  #ifdef ESP32
    esp_sleep_enable_timer_wakeup(30000000);  // 30 seconds
    esp_light_sleep_start();
  #elif defined(ESP8266)
    ESP.deepSleep(30000000);  // 30 seconds
  #else
    // Arduino sleep mode
    delay(30000);
  #endif
}
```

### Adaptive WOR Periods
```cpp
void adaptiveWOR() {
  static int messageCount = 0;
  static unsigned long lastAdapt = 0;
  
  // Adapt WOR period based on message frequency
  if (millis() - lastAdapt > 300000) {  // Every 5 minutes
    uint8_t newPeriod;
    
    if (messageCount > 10) {
      newPeriod = WOR_500_000;   // High traffic - fast wake
    } else if (messageCount > 2) {
      newPeriod = WOR_2000_000;  // Medium traffic - normal wake
    } else {
      newPeriod = WOR_4000_000;  // Low traffic - slow wake
    }
    
    configureWORPeriod(newPeriod);
    messageCount = 0;
    lastAdapt = millis();
  }
}
```

## Troubleshooting WOR

### No WOR Reception
1. **Check WOR Period**: Transmitter and receiver must match
2. **Verify Preamble**: Ensure sufficient preamble time
3. **Test Timing**: Use oscilloscope to verify wake cycles
4. **Power Supply**: Ensure stable power during wake/sleep

### Missed Messages
1. **Increase Preamble**: Extend transmission time
2. **Reduce WOR Period**: Wake more frequently
3. **Synchronize Clocks**: Use network time sync
4. **Add Redundancy**: Send important messages multiple times

### High Power Consumption
1. **Check Sleep Current**: Verify μA sleep consumption
2. **Optimize Wake Period**: Use longest acceptable period
3. **Minimize Active Time**: Process messages quickly
4. **Fix Hardware**: Check for current leaks

## Example Projects

### Solar-Powered Weather Station
```cpp
void solarWeatherStation() {
  // Read weather sensors
  WeatherData data = {
    .temperature = readTemperature(),
    .humidity = readHumidity(),
    .pressure = readPressure(),
    .battery = readBatteryVoltage(),
    .solar = readSolarVoltage()
  };
  
  // Send via WOR (very low power)
  e220ttl.sendFixedMessage(0x01, 0x00, 23, &data, sizeof(data));
  
  // Sleep for 10 minutes
  ESP.deepSleep(600000000);
}
```

### Emergency Alert System
```cpp
void emergencyAlert() {
  String alert = "EMERGENCY: Flood detected at sensor 05";
  
  // Send high-priority WOR message with retries
  for (int i = 0; i < 5; i++) {
    e220ttl.sendBroadcastFixedMessage(23, alert);
    delay(1000);  // 1 second between retries
  }
  
  Serial.println("🚨 Emergency alert transmitted");
}
```

## Best Practices

### WOR Design Guidelines
- Match WOR periods between transmitter and receiver
- Allow sufficient preamble time for reliable wake-up
- Process messages quickly to minimize active time
- Use fixed addressing for reliable routing
- Implement message acknowledgment for critical data

### Power Management
- Combine E220 WOR with microcontroller sleep modes
- Monitor battery voltage and adapt transmission frequency
- Use voltage regulators with low quiescent current
- Design for temperature variations affecting battery life

### Network Architecture
- Use star topology with WOR gateway for collection
- Implement time synchronization for coordinated wake-up
- Design message formats for minimal air time
- Plan for message collision avoidance

## Related Documentation

- [Configuration Guide](configuration.md) - WOR parameter details
- [Fixed Mode](fixed-mode.md) - Addressing with WOR
- [Transparent Mode](transparent-mode.md) - Simple WOR usage
- [API Reference](api-reference.md) - WOR function documentation
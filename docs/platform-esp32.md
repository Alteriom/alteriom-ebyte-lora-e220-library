# ESP32 Platform Setup

## Overview

ESP32 is an excellent platform for E220 development, offering native 3.3V operation, multiple hardware UARTs, WiFi/Bluetooth connectivity, and powerful processing capabilities. No voltage conversion is needed.

## Advantages of ESP32

- ✅ **Native 3.3V**: Direct connection to E220
- ✅ **Multiple UARTs**: Hardware Serial1, Serial2 available
- ✅ **High Performance**: Dual-core 240MHz processor
- ✅ **WiFi + Bluetooth**: IoT connectivity
- ✅ **Large Memory**: 520KB RAM, 4MB+ Flash
- ✅ **Deep Sleep**: Ultra-low power modes

## Hardware Requirements

### Pin Compatibility
ESP32 has flexible GPIO mapping - most pins can be used for UART

### Power Specifications
- **Operating Voltage**: 3.3V (perfect for E220)
- **Current Capability**: Up to 1A (sufficient for E220)
- **Sleep Current**: ~10μA (excellent for battery applications)

## Basic Wiring

### ESP32 DevKit to E220 (Recommended)
```
E220 Module     ESP32 DevKit
-----------     -------------
VCC       ←─────── 3.3V
GND       ←─────── GND
TX        ←─────── GPIO16 (Serial2 RX)
RX        ←─────── GPIO17 (Serial2 TX)
M0        ←─────── GPIO18
M1        ←─────── GPIO19
AUX       ←─────── GPIO21
```

### Alternative Pin Configuration
```cpp
// ESP32 has flexible GPIO - use any available pins
E220 Module     ESP32 Alternative
-----------     ------------------
VCC       ←─────── 3.3V
GND       ←─────── GND
TX        ←─────── GPIO25
RX        ←─────── GPIO26
M0        ←─────── GPIO32
M1        ←─────── GPIO33
AUX       ←─────── GPIO27
```

## Software Setup

### Arduino IDE Configuration

1. **Install ESP32 Board Package**:
   - Open Arduino IDE
   - Go to **File** → **Preferences**
   - Add to Additional Board Manager URLs:
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Go to **Tools** → **Board** → **Boards Manager**
   - Search "ESP32" and install "ESP32 by Espressif Systems"

2. **Select Board**:
   - **Tools** → **Board** → **ESP32 Dev Module**
   - **Tools** → **Port** → Select your ESP32 port

### Basic Code Template
```cpp
#include "LoRa_E220.h"

// ESP32 pin definitions
#define E220_M0  18
#define E220_M1  19
#define E220_AUX 21

// Use hardware Serial2
LoRa_E220 e220ttl(&Serial2, E220_M0, E220_M1, E220_AUX);

void setup() {
  Serial.begin(115200);   // USB serial
  Serial2.begin(9600);    // E220 communication
  
  delay(500);
  e220ttl.begin();
  
  Serial.println("ESP32 + E220 Setup Complete");
  
  // Print network information
  printNetworkInfo();
}

void loop() {
  // Check for LoRa messages
  if (e220ttl.available()) {
    String message = e220ttl.receiveMessage();
    Serial.println("LoRa: " + message);
  }
  
  // Send periodic message
  static unsigned long lastSend = 0;
  if (millis() - lastSend > 30000) {  // Every 30 seconds
    String status = "ESP32 Status: " + String(millis());
    e220ttl.sendMessage(status);
    lastSend = millis();
  }
}

void printNetworkInfo() {
  ResponseStructContainer c = e220ttl.getConfiguration();
  if (c.status.code == 1) {
    Serial.println("✅ E220 Configuration:");
    Serial.println("Address: " + String(c.data.ADDH) + ":" + String(c.data.ADDL));
    Serial.println("Channel: " + String(c.data.CHAN));
  }
  c.close();
}
```

## ESP32-Specific Features

### WiFi + LoRa Gateway
```cpp
#include <WiFi.h>
#include <WebServer.h>
#include "LoRa_E220.h"

const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

WebServer server(80);
LoRa_E220 e220ttl(&Serial2, 18, 19, 21);

void setup() {
  Serial.begin(115200);
  e220ttl.begin();
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("WiFi connected: " + WiFi.localIP().toString());
  
  // Web server routes
  server.on("/", handleRoot);
  server.on("/send", handleSend);
  server.begin();
}

void loop() {
  server.handleClient();
  
  // Forward LoRa messages to web clients
  if (e220ttl.available()) {
    String message = e220ttl.receiveMessage();
    lastLoRaMessage = message;
    Serial.println("LoRa→WiFi: " + message);
  }
}

void handleSend() {
  if (server.hasArg("message")) {
    String message = server.arg("message");
    e220ttl.sendMessage(message);
    server.send(200, "text/plain", "Message sent: " + message);
  }
}
```

### Bluetooth + LoRa Bridge
```cpp
#include "BluetoothSerial.h"
#include "LoRa_E220.h"

BluetoothSerial SerialBT;
LoRa_E220 e220ttl(&Serial2, 18, 19, 21);

void setup() {
  Serial.begin(115200);
  e220ttl.begin();
  
  SerialBT.begin("ESP32_LoRa_Bridge");
  Serial.println("Bluetooth device ready to pair");
}

void loop() {
  // Bluetooth → LoRa
  if (SerialBT.available()) {
    String message = SerialBT.readString();
    message.trim();
    e220ttl.sendMessage(message);
    SerialBT.println("Sent via LoRa: " + message);
  }
  
  // LoRa → Bluetooth
  if (e220ttl.available()) {
    String message = e220ttl.receiveMessage();
    SerialBT.println("LoRa: " + message);
    Serial.println("LoRa→BT: " + message);
  }
}
```

### Deep Sleep with LoRa Wake-up
```cpp
#include "LoRa_E220.h"

RTC_DATA_ATTR int bootCount = 0;
LoRa_E220 e220ttl(&Serial2, 18, 19, 21);

void setup() {
  Serial.begin(115200);
  bootCount++;
  
  Serial.println("Boot #" + String(bootCount));
  
  e220ttl.begin();
  
  // Send sensor data
  sendSensorData();
  
  // Configure wake-up (10 minutes)
  esp_sleep_enable_timer_wakeup(10 * 60 * 1000000ULL);
  
  Serial.println("Going to sleep...");
  esp_deep_sleep_start();
}

void loop() {
  // Never reached due to deep sleep
}

void sendSensorData() {
  // Read built-in temperature sensor
  float temp = temperatureRead();
  
  // Create JSON message
  String message = "{\"device\":\"ESP32\",\"boot\":" + String(bootCount) + 
                   ",\"temp\":" + String(temp) + ",\"time\":" + String(millis()) + "}";
  
  ResponseStatus rs = e220ttl.sendMessage(message);
  
  if (rs.code == 1) {
    Serial.println("✅ Data sent: " + message);
  } else {
    Serial.println("❌ Send failed");
  }
}
```

## Performance Optimization

### High-Speed Communication
```cpp
void highSpeedSetup() {
  // Use high-speed UART
  Serial2.begin(115200);
  
  // Configure E220 for high air data rate
  ResponseStructContainer c = e220ttl.getConfiguration();
  Configuration config = c.data;
  
  config.SPED.uartBaudRate = UART_BPS_115200;
  config.SPED.airDataRate = AIR_DATA_RATE_62_5;
  config.SPED.uartParity = MODE_00_8N1;
  
  e220ttl.setConfiguration(config, WRITE_CFG_PWR_DWN_SAVE);
  c.close();
  
  Serial.println("High-speed LoRa configured");
}
```

### Interrupt-Driven Reception
```cpp
volatile bool messageReceived = false;

void setup() {
  // Configure AUX pin interrupt
  pinMode(21, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(21), auxInterrupt, FALLING);
}

void IRAM_ATTR auxInterrupt() {
  messageReceived = true;
}

void loop() {
  if (messageReceived) {
    messageReceived = false;
    
    // Small delay for message to arrive
    delay(10);
    
    if (e220ttl.available()) {
      String message = e220ttl.receiveMessage();
      processMessage(message);
    }
  }
}
```

### Multi-Core Processing
```cpp
TaskHandle_t LoRaTask;

void setup() {
  Serial.begin(115200);
  e220ttl.begin();
  
  // Create LoRa task on Core 0
  xTaskCreatePinnedToCore(
    loRaTask,     // Function
    "LoRaTask",   // Name
    10000,        // Stack size
    NULL,         // Parameters
    1,            // Priority
    &LoRaTask,    // Task handle
    0             // Core 0
  );
  
  Serial.println("Multi-core LoRa setup complete");
}

void loop() {
  // Main loop runs on Core 1
  // Handle WiFi, web server, etc.
  handleWiFiOperations();
  delay(100);
}

void loRaTask(void * pvParameters) {
  // LoRa operations run on Core 0
  for (;;) {
    if (e220ttl.available()) {
      String message = e220ttl.receiveMessage();
      
      // Use queue to send to main task
      xQueueSend(messageQueue, &message, portMAX_DELAY);
    }
    
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
```

## Power Management

### Battery Monitoring
```cpp
void monitorBattery() {
  // Read battery voltage (if using voltage divider)
  int batteryReading = analogRead(35);  // ADC1_CH7
  float batteryVoltage = (batteryReading / 4095.0) * 3.3 * 2;  // Adjust for divider
  
  Serial.println("Battery: " + String(batteryVoltage) + "V");
  
  if (batteryVoltage < 3.2) {
    Serial.println("⚠️ Low battery - entering deep sleep");
    esp_deep_sleep_start();
  }
}
```

### Power-Optimized Sensor Node
```cpp
void powerOptimizedLoop() {
  // Wake up, read sensors, transmit, sleep
  float temperature = readTemperatureSensor();
  float humidity = readHumiditySensor();
  
  // Create efficient message
  char message[50];
  sprintf(message, "T:%.1f,H:%.1f,V:%.2f", temperature, humidity, readBatteryVoltage());
  
  // Send via LoRa
  e220ttl.sendMessage(message);
  
  // Wait for transmission to complete
  delay(1000);
  
  // Sleep for 5 minutes
  esp_sleep_enable_timer_wakeup(5 * 60 * 1000000ULL);
  esp_deep_sleep_start();
}
```

## IoT Integration Examples

### MQTT Gateway
```cpp
#include <WiFi.h>
#include <PubSubClient.h>
#include "LoRa_E220.h"

WiFiClient espClient;
PubSubClient mqtt(espClient);
LoRa_E220 e220ttl(&Serial2, 18, 19, 21);

const char* mqtt_server = "your_mqtt_broker.com";

void setup() {
  Serial.begin(115200);
  e220ttl.begin();
  
  setupWiFi();
  mqtt.setServer(mqtt_server, 1883);
  mqtt.setCallback(mqttCallback);
}

void loop() {
  if (!mqtt.connected()) {
    reconnectMQTT();
  }
  mqtt.loop();
  
  // LoRa → MQTT
  if (e220ttl.available()) {
    String message = e220ttl.receiveMessage();
    String topic = "lora/sensors/" + extractDeviceId(message);
    mqtt.publish(topic.c_str(), message.c_str());
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  // MQTT → LoRa
  String message = String((char*)payload).substring(0, length);
  e220ttl.sendMessage(message);
}
```

### HTTP REST API
```cpp
#include <WiFi.h>
#include <HTTPClient.h>
#include "LoRa_E220.h"

LoRa_E220 e220ttl(&Serial2, 18, 19, 21);

void setup() {
  Serial.begin(115200);
  e220ttl.begin();
  setupWiFi();
}

void loop() {
  if (e220ttl.available()) {
    String message = e220ttl.receiveMessage();
    
    // Parse sensor data
    float temperature = extractTemperature(message);
    String deviceId = extractDeviceId(message);
    
    // Send to cloud API
    sendToCloudAPI(deviceId, temperature);
  }
}

void sendToCloudAPI(String deviceId, float temperature) {
  HTTPClient http;
  http.begin("https://api.yourservice.com/sensors/" + deviceId);
  http.addHeader("Content-Type", "application/json");
  
  String payload = "{\"temperature\":" + String(temperature) + 
                   ",\"timestamp\":\"" + getTimestamp() + "\"}";
  
  int httpResponseCode = http.POST(payload);
  
  if (httpResponseCode > 0) {
    Serial.println("✅ Data sent to cloud");
  } else {
    Serial.println("❌ Cloud upload failed");
  }
  
  http.end();
}
```

## Troubleshooting ESP32

### Common Issues

**Serial Monitor Shows Gibberish**:
- Check baud rate (115200 for ESP32)
- Verify correct COM port selected
- Try different USB cable

**E220 Not Responding**:
- Check 3.3V power connection
- Verify GPIO pin assignments
- Test with simple connectivity code

**WiFi Connection Failed**:
- Check SSID and password
- Verify 2.4GHz network (ESP32 doesn't support 5GHz)
- Check signal strength

### Debug Techniques
```cpp
void debugESP32System() {
  Serial.println("=== ESP32 System Info ===");
  Serial.println("Chip Model: " + String(ESP.getChipModel()));
  Serial.println("CPU Frequency: " + String(ESP.getCpuFreqMHz()) + " MHz");
  Serial.println("Free Heap: " + String(ESP.getFreeHeap()) + " bytes");
  Serial.println("Flash Size: " + String(ESP.getFlashChipSize()) + " bytes");
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi IP: " + WiFi.localIP().toString());
    Serial.println("WiFi RSSI: " + String(WiFi.RSSI()) + " dBm");
  }
}
```

## Best Practices

### Hardware Design
- Use ESP32 DevKit for prototyping
- Add external antenna connector for better range
- Include battery monitoring circuit
- Use voltage regulator for stable 3.3V supply

### Software Architecture
- Use RTOS tasks for concurrent operations
- Implement proper error handling and recovery
- Use interrupts for real-time LoRa reception
- Optimize power consumption for battery applications

### Network Integration
- Implement secure communication (HTTPS, TLS)
- Use JSON for structured data exchange
- Add device authentication and encryption
- Handle network disconnections gracefully

## Related Documentation

- [Wiring Guide](wiring.md) - Hardware connections
- [WiFi Integration Examples](examples.md) - IoT projects
- [Power Management](wor.md) - Battery optimization
- [API Reference](api-reference.md) - Function documentation
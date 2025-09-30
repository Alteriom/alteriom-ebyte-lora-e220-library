# ESP8266 Platform Setup

## Overview

ESP8266 is a popular, low-cost WiFi microcontroller that works well with the E220 LoRa module. It operates at 3.3V (compatible with E220) and offers WiFi connectivity for IoT applications, though with more limited resources than ESP32.

## Advantages of ESP8266

- ✅ **Native 3.3V**: Direct connection to E220
- ✅ **WiFi Built-in**: IoT connectivity 
- ✅ **Low Cost**: Very affordable platform
- ✅ **Arduino Compatible**: Easy programming
- ✅ **Low Power**: Deep sleep support

## Limitations

- ⚠️ **Limited Pins**: Only ~11 usable GPIO pins
- ⚠️ **Single Core**: No parallel processing
- ⚠️ **Limited RAM**: ~80KB available
- ⚠️ **One Hardware UART**: Must use SoftwareSerial

## Hardware Requirements

### Pin Mapping (NodeMCU)
```
NodeMCU Pin     GPIO     Function
-----------     ----     --------
D0              16       Wake from deep sleep
D1              5        General I/O
D2              4        General I/O  
D3              0        Boot mode (avoid)
D4              2        Boot mode (built-in LED)
D5              14       General I/O
D6              12       General I/O
D7              13       General I/O
D8              15       Boot mode (avoid)
A0              ADC      Analog input only
```

## Basic Wiring

### ESP8266 NodeMCU to E220
```
E220 Module     NodeMCU (ESP8266)
-----------     ------------------
VCC       ←─────── 3.3V
GND       ←─────── GND  
TX        ←─────── D1 (GPIO5) 
RX        ←─────── D2 (GPIO4)
M0        ←─────── D5 (GPIO14)
M1        ←─────── D6 (GPIO12)
AUX       ←─────── D7 (GPIO13)
```

### Alternative Configuration
```
E220 Module     NodeMCU Alternative
-----------     --------------------
VCC       ←─────── 3.3V
GND       ←─────── GND
TX        ←─────── D5 (GPIO14)
RX        ←─────── D6 (GPIO12) 
M0        ←─────── D1 (GPIO5)
M1        ←─────── D2 (GPIO4)
AUX       ←─────── D7 (GPIO13)
```

## Software Setup

### Arduino IDE Configuration

1. **Install ESP8266 Board Package**:
   - Open Arduino IDE
   - Go to **File** → **Preferences**
   - Add to Additional Board Manager URLs:
     ```
     http://arduino.esp8266.com/stable/package_esp8266com_index.json
     ```
   - Go to **Tools** → **Board** → **Boards Manager**
   - Search "esp8266" and install "esp8266 by ESP8266 Community"

2. **Select Board**:
   - **Tools** → **Board** → **NodeMCU 1.0 (ESP-12E Module)**
   - **Tools** → **Flash Size** → **4MB (FS:2MB OTA:~1019KB)**
   - **Tools** → **Port** → Select your ESP8266 port

### Basic Code Template
```cpp
#include "LoRa_E220.h"
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

// Pin definitions for NodeMCU
#define E220_RX  D1  // GPIO5
#define E220_TX  D2  // GPIO4
#define E220_M0  D5  // GPIO14
#define E220_M1  D6  // GPIO12
#define E220_AUX D7  // GPIO13

// Create SoftwareSerial for E220
SoftwareSerial e220Serial(E220_RX, E220_TX);
LoRa_E220 e220ttl(&e220Serial, E220_M0, E220_M1, E220_AUX);

void setup() {
  Serial.begin(115200);
  e220Serial.begin(9600);
  
  delay(500);
  e220ttl.begin();
  
  Serial.println("ESP8266 + E220 Setup Complete");
  
  // Print configuration
  printConfiguration();
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
    String status = "ESP8266 alive: " + String(millis());
    e220ttl.sendMessage(status);
    lastSend = millis();
  }
}

void printConfiguration() {
  ResponseStructContainer c = e220ttl.getConfiguration();
  if (c.status.code == 1) {
    Serial.println("✅ E220 Configuration:");
    Serial.println("Address: " + String(c.data.ADDH) + ":" + String(c.data.ADDL));
    Serial.println("Channel: " + String(c.data.CHAN));
  } else {
    Serial.println("❌ Configuration read failed");
  }
  c.close();
}
```

## ESP8266-Specific Features

### WiFi + LoRa Gateway
```cpp
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "LoRa_E220.h"
#include <SoftwareSerial.h>

const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

ESP8266WebServer server(80);
SoftwareSerial e220Serial(D1, D2);
LoRa_E220 e220ttl(&e220Serial, D5, D6, D7);

String lastLoRaMessage = "";

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
  server.on("/", HTTP_GET, handleRoot);
  server.on("/send", HTTP_GET, handleSend);
  server.on("/status", HTTP_GET, handleStatus);
  server.begin();
  
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
  
  // Process LoRa messages
  if (e220ttl.available()) {
    lastLoRaMessage = e220ttl.receiveMessage();
    Serial.println("LoRa→WiFi: " + lastLoRaMessage);
  }
  
  yield();  // Important for ESP8266 stability
}

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>ESP8266 LoRa Gateway</h1>";
  html += "<p>Last LoRa Message: " + lastLoRaMessage + "</p>";
  html += "<form action='/send'>";
  html += "Message: <input type='text' name='message'>";
  html += "<input type='submit' value='Send'>";
  html += "</form></body></html>";
  
  server.send(200, "text/html", html);
}

void handleSend() {
  if (server.hasArg("message")) {
    String message = server.arg("message");
    e220ttl.sendMessage(message);
    server.send(200, "text/plain", "✅ Sent: " + message);
  } else {
    server.send(400, "text/plain", "❌ No message parameter");
  }
}

void handleStatus() {
  String status = "{";
  status += "\"uptime\":" + String(millis()) + ",";
  status += "\"free_heap\":" + String(ESP.getFreeHeap()) + ",";
  status += "\"wifi_rssi\":" + String(WiFi.RSSI()) + ",";
  status += "\"last_message\":\"" + lastLoRaMessage + "\"";
  status += "}";
  
  server.send(200, "application/json", status);
}
```

### Deep Sleep Sensor Node
```cpp
#include "LoRa_E220.h"
#include <SoftwareSerial.h>

// RTC memory survives deep sleep
uint32_t rtc_count __attribute__((section(".rtc.data")));

SoftwareSerial e220Serial(D1, D2);
LoRa_E220 e220ttl(&e220Serial, D5, D6, D7);

void setup() {
  Serial.begin(115200);
  Serial.println("ESP8266 waking up...");
  
  rtc_count++;
  Serial.println("Boot count: " + String(rtc_count));
  
  e220ttl.begin();
  
  // Read sensor and send data
  sendSensorData();
  
  // Sleep for 5 minutes (5 * 60 * 1000000 microseconds)
  Serial.println("Going to sleep...");
  ESP.deepSleep(5 * 60 * 1000000);
}

void loop() {
  // Never reached due to deep sleep
}

void sendSensorData() {
  // Read analog sensor (voltage divider for >3.3V signals)
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (3.3 / 1024.0);
  
  // Create message
  String message = "ESP8266," + String(rtc_count) + "," + String(voltage, 2);
  
  ResponseStatus rs = e220ttl.sendMessage(message);
  
  if (rs.code == 1) {
    Serial.println("✅ Data sent: " + message);
  } else {
    Serial.println("❌ Send failed");
  }
  
  delay(1000);  // Allow transmission to complete
}
```

## Performance Optimization

### Memory Management
```cpp
void optimizeMemory() {
  // Monitor free heap
  Serial.println("Free heap: " + String(ESP.getFreeHeap()));
  
  // Use F() macro for string constants
  Serial.println(F("This string is stored in flash"));
  
  // Avoid String objects in loops
  char buffer[50];
  sprintf(buffer, "Value: %d", analogRead(A0));
  
  // Close ResponseStructContainer promptly
  ResponseStructContainer c = e220ttl.getConfiguration();
  // Use data immediately
  int channel = c.data.CHAN;
  c.close();  // Free memory
  
  Serial.println("Channel: " + String(channel));
}
```

### SoftwareSerial Limitations
```cpp
void handleSoftwareSerialLimitations() {
  // SoftwareSerial is not as reliable as hardware serial
  // Maximum recommended baud rate: 57600
  
  // Use 9600 baud for reliable communication
  SoftwareSerial e220Serial(D1, D2);
  e220Serial.begin(9600);  // Stick to 9600 for stability
  
  // Add delays for stability
  delay(100);
  
  // Check for buffer overflow
  if (e220Serial.available() > 60) {
    Serial.println("⚠️ SoftwareSerial buffer near full");
    // Read and discard old data
    while (e220Serial.available()) {
      e220Serial.read();
    }
  }
}
```

### WiFi Power Management
```cpp
void optimizeWiFiPower() {
  // For battery applications, manage WiFi power
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  // Send data quickly
  sendDataToServer();
  
  // Disconnect WiFi to save power
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  
  // Use LoRa for local communication
  e220ttl.sendMessage("Data uploaded to WiFi");
}
```

## Common ESP8266 Issues

### Stability Problems
```cpp
void improveStability() {
  // Add yield() in long loops
  while (condition) {
    // Your code here
    yield();  // Prevents watchdog reset
  }
  
  // Avoid blocking delays > 1 second
  unsigned long startTime = millis();
  while (millis() - startTime < 5000) {
    // Do other work
    yield();
  }
  
  // Use shorter delays with yield
  for (int i = 0; i < 10; i++) {
    delay(500);
    yield();
  }
}
```

### Boot Issues
```cpp
void handleBootIssues() {
  // Avoid D3 (GPIO0) and D8 (GPIO15) for critical functions
  // These pins affect boot mode
  
  // D3 (GPIO0) must be HIGH at boot
  // D8 (GPIO15) must be LOW at boot
  
  // Safe pins for E220 connections:
  // D1 (GPIO5), D2 (GPIO4), D5 (GPIO14), D6 (GPIO12), D7 (GPIO13)
  
  Serial.println("Boot successful - GPIO states OK");
}
```

### Memory Issues
```cpp
void handleMemoryIssues() {
  // Monitor heap fragmentation
  uint32_t free = ESP.getFreeHeap();
  uint16_t max_block = ESP.getMaxFreeBlockSize();
  
  Serial.println("Free heap: " + String(free));
  Serial.println("Max block: " + String(max_block));
  
  if (max_block < (free * 0.5)) {
    Serial.println("⚠️ Heap fragmentation detected");
    // Consider restarting: ESP.restart();
  }
}
```

## IoT Examples

### ThingSpeak Data Logger
```cpp
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "LoRa_E220.h"

const char* ssid = "your_wifi";
const char* password = "your_password";
const char* thingspeak_key = "your_api_key";

SoftwareSerial e220Serial(D1, D2);
LoRa_E220 e220ttl(&e220Serial, D5, D6, D7);

void setup() {
  Serial.begin(115200);
  e220ttl.begin();
  setupWiFi();
}

void loop() {
  if (e220ttl.available()) {
    String data = e220ttl.receiveMessage();
    
    // Parse sensor data (format: "SENSOR,temp,humidity")
    if (data.startsWith("SENSOR,")) {
      float temp = extractTemperature(data);
      float humidity = extractHumidity(data);
      
      uploadToThingSpeak(temp, humidity);
    }
  }
  
  delay(100);
  yield();
}

void uploadToThingSpeak(float temp, float humidity) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://api.thingspeak.com/update?api_key=" + String(thingspeak_key);
    url += "&field1=" + String(temp);
    url += "&field2=" + String(humidity);
    
    http.begin(url);
    int responseCode = http.GET();
    
    if (responseCode > 0) {
      Serial.println("✅ Data uploaded to ThingSpeak");
    } else {
      Serial.println("❌ Upload failed");
    }
    
    http.end();
  }
}
```

### MQTT Sensor Bridge
```cpp
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "LoRa_E220.h"

WiFiClient espClient;
PubSubClient mqtt(espClient);
SoftwareSerial e220Serial(D1, D2);
LoRa_E220 e220ttl(&e220Serial, D5, D6, D7);

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
  
  // LoRa to MQTT
  if (e220ttl.available()) {
    String message = e220ttl.receiveMessage();
    String topic = "lora/sensors/" + extractDeviceId(message);
    mqtt.publish(topic.c_str(), message.c_str());
  }
  
  yield();
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  // MQTT to LoRa
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  e220ttl.sendMessage(message);
  Serial.println("MQTT→LoRa: " + message);
}
```

## Best Practices for ESP8266

### Hardware Design
- Use NodeMCU or Wemos D1 Mini for prototyping
- Add external antenna for better WiFi range
- Use 3.3V regulator with adequate current capability
- Avoid using boot-mode pins for critical functions

### Software Development
- Always use yield() in long loops
- Monitor free heap memory regularly
- Use F() macro for string constants
- Implement proper error handling and recovery

### Power Management
- Use deep sleep for battery applications
- Manage WiFi power consumption
- Monitor battery voltage
- Implement watchdog timer for reliability

### Network Integration
- Use lightweight protocols (MQTT, HTTP GET)
- Implement connection retry logic
- Handle network timeouts gracefully
- Use JSON for structured data exchange

## Related Documentation

- [Wiring Guide](wiring.md) - Hardware connections
- [Power Management](wor.md) - Battery optimization
- [WiFi Examples](examples.md) - IoT integration
- [Troubleshooting](troubleshooting.md) - Common issues
# FAQ - Frequently Asked Questions

## General Questions

### What is the E220 LoRa module?
The E220 series is a family of LoRa radio modules based on the LLCC68 chipset, designed for long-range (5-10km) wireless communication at 410-441MHz frequency range.

### Which E220 variants are supported?
All E220 series modules are supported:
- E220-400T22D (22dBm, 170mW)
- E220-400T30D (30dBm, 1W)
- E220-400T22S (SMD version)
- Other variants in the E220-400 series

### What platforms are supported?
- Arduino (Uno, Nano, Mega, Leonardo)
- ESP32 (all variants)
- ESP8266 (NodeMCU, Wemos D1)
- STM32 (Nucleo, BluePill)
- Raspberry Pi Pico
- SAMD (Arduino Zero, MKR series)

## Hardware Questions

### Can I use 5V Arduino with E220?
⚠️ **Caution**: E220 operates at 3.3V only. For 5V Arduino boards:
- Use external 3.3V regulator for power
- Use voltage dividers or level shifters for signal pins
- Never connect 5V directly to E220

### What's the maximum range?
- **Line of sight**: 5-10km (varies by terrain, antenna, power setting)
- **Urban environment**: 1-3km
- **Indoor**: 100-500m through walls
- Factors: antenna, power level, obstacles, interference

### How much power does it consume?
- **Sleep mode**: ~2μA
- **Receive mode**: ~12mA
- **Transmit mode**: 120mA (22dBm) to 900mA (30dBm)
- **Configuration mode**: ~15mA

### Do I need an antenna?
⚠️ **Yes, always!** Never operate without antenna:
- Minimum: 1/4 wave wire antenna (~17cm for 433MHz)
- Better: Duck antenna (2-5dBi gain)
- Best: High-gain directional antenna
- **Never transmit without antenna** - can damage the module

## Software Questions

### Which Arduino IDE version do I need?
- **Minimum**: Arduino IDE 1.8.0
- **Recommended**: Arduino IDE 2.0 or newer
- **PlatformIO**: Also supported

### How do I install the library?
1. Open Arduino IDE
2. Go to **Sketch** → **Include Library** → **Manage Libraries**
3. Search "EByte LoRa E220"
4. Install the library by Renzo Mischianti

### What baud rate should I use?
- **Default**: 9600 baud (most reliable)
- **Arduino Uno**: Stick to 9600 with SoftwareSerial
- **ESP32**: Can use up to 115200 with hardware serial
- **ESP8266**: 9600 recommended with SoftwareSerial

## Communication Questions

### What's the difference between transparent and fixed mode?
- **Transparent mode**: Simple point-to-point, no addressing
- **Fixed mode**: Addressed packets, multiple devices, routing capability

### How many devices can communicate?
- **Transparent mode**: 2 devices (point-to-point)
- **Fixed mode**: Up to 65,536 addresses (256 × 256)
- **Channels**: 80 available channels for separation

### Can I use WiFi and LoRa together?
Yes! Popular combinations:
- **ESP32**: WiFi gateway + LoRa sensors
- **ESP8266**: WiFi bridge + LoRa nodes
- Use one for local (LoRa) and one for internet (WiFi)

### What about encryption?
- **Hardware encryption**: Not available on E220
- **Software encryption**: Implement in your application
- **Recommendations**: AES encryption, rolling codes, authentication

## Troubleshooting Questions

### Module doesn't respond to configuration commands
1. Check power supply (must be 3.3V)
2. Verify wiring connections
3. Set M0=HIGH, M1=HIGH for configuration mode
4. Wait for AUX pin to go HIGH before sending commands
5. Check baud rate settings

### No messages received
1. Verify both devices on same channel
2. Check address configuration (fixed mode)
3. Test with devices close together
4. Monitor RSSI for signal strength
5. Check antenna connections

### Messages are garbled
1. Check UART baud rate matching
2. Verify air data rate settings
3. Check for electromagnetic interference
4. Reduce transmission power if too close
5. Check for buffer overflow

### Poor range/performance
1. Check antenna connection and type
2. Increase transmission power setting
3. Verify clear line of sight
4. Check for interference sources
5. Test at different times of day

## Battery/Power Questions

### How long will batteries last?
Depends on usage pattern:
- **Continuous receive**: ~7 days (2000mAh battery)
- **WOR mode**: Several months to years
- **Deep sleep**: Up to 45 years (theoretical)

### Best practices for battery power?
1. Use WOR (Wake-on-Radio) mode
2. Implement microcontroller deep sleep
3. Monitor battery voltage
4. Use efficient power regulators
5. Optimize transmission frequency

### Can I use solar power?
Yes! Excellent for remote sensors:
- Small solar panel (5-10W)
- Battery backup (LiPo/LiFePO4)
- Charge controller
- Low-power design with WOR

## Programming Questions

### How do I send structured data?
```cpp
struct SensorData {
  float temperature;
  float humidity;
  uint32_t timestamp;
};

SensorData data = {23.5, 65.2, millis()};
e220ttl.sendFixedMessage(0x01, 0x00, 23, &data, sizeof(data));
```

### How do I implement acknowledgments?
```cpp
bool sendWithAck(String message) {
  e220ttl.sendMessage(message);
  
  // Wait for ACK
  unsigned long start = millis();
  while (millis() - start < 2000) {
    if (e220ttl.available()) {
      String response = e220ttl.receiveMessage();
      if (response == "ACK") return true;
    }
  }
  return false;  // Timeout
}
```

### How do I handle errors?
```cpp
ResponseStructContainer c = e220ttl.getConfiguration();
if (c.status.code == 1) {
  // Success - use c.data
  Configuration config = c.data;
} else {
  // Error - check c.status.getResponseDescription()
  Serial.println("Error: " + c.status.getResponseDescription());
}
c.close();  // Always close!
```

## Advanced Questions

### Can I make a mesh network?
Not directly, but you can implement:
- Star topology with central gateway
- Simple relay/repeater nodes
- Message routing in software
- Consider dedicated LoRaWAN for complex mesh

### How do I optimize for speed?
1. Use hardware serial instead of SoftwareSerial
2. Match UART speed to air data rate
3. Use higher air data rates for short range
4. Minimize message size
5. Use interrupts for reception

### Integration with cloud services?
Yes! Common patterns:
- **MQTT**: LoRa → ESP32/ESP8266 → WiFi → MQTT broker
- **HTTP**: LoRa → Gateway → REST API → Cloud
- **LoRaWAN**: Use dedicated LoRaWAN gateway for TTN/AWS IoT

## Still Need Help?

1. Check the [Troubleshooting Guide](troubleshooting.md)
2. Review [Examples Library](examples.md)
3. Read the [API Reference](api-reference.md)
4. Visit [GitHub Issues](https://github.com/xreef/EByte_LoRa_E220_Series_Library/issues)
5. Post detailed questions with:
   - Hardware setup
   - Complete code
   - Error messages
   - Serial output
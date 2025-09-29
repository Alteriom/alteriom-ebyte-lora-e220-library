# ⚙️ Configuration Guide

Complete guide to configuring your EByte E220 LoRa device for optimal performance.

## 📋 Configuration Overview

The EByte E220 device stores configuration parameters that control its operation. These parameters can be modified to optimize performance for your specific application.

## 🏗️ Configuration Structure

```cpp
struct Configuration {
    byte ADDH;      // Address high byte (0x00-0xFF)
    byte ADDL;      // Address low byte (0x00-0xFF)  
    byte SPED;      // Speed configuration
    byte CHAN;      // Channel (0-83)
    byte OPTION;    // Option settings
};
```

## 📡 Address Configuration (ADDH, ADDL)

The device address is a 16-bit value formed by combining ADDH (high byte) and ADDL (low byte).

### Address Range
- **ADDH**: 0x00 to 0xFF (0-255)
- **ADDL**: 0x00 to 0xFF (0-255)
- **Combined**: 0x0000 to 0xFFFF (0-65535)

### Special Addresses
- **0xFFFF**: Broadcast address (all devices receive)
- **0x0000**: Valid device address
- **Custom**: Any other combination for specific devices

### Example Configuration
```cpp
Configuration config;
config.ADDH = 0x00;    // High byte
config.ADDL = 0x01;    // Low byte
// Resulting address: 0x0001
```

### Address Usage
- **Transparent Mode**: Address is ignored, all devices on same channel communicate
- **Fixed Mode**: Messages are sent to specific addresses
- **Broadcast**: Use 0xFFFF to send to all devices

## 📻 Channel Configuration (CHAN)

Channels control the radio frequency used for communication.

### Channel Range
- **Valid Range**: 0-83 (84 total channels)
- **Frequency**: 410.125 MHz + CHAN × 1 MHz
- **Example**: Channel 23 = 433.125 MHz

### Channel Selection Guidelines

| Channel | Frequency (MHz) | Common Use |
|---------|----------------|------------|
| 0 | 410.125 | Testing |
| 15 | 425.125 | General purpose |
| 23 | 433.125 | ISM band (popular) |
| 35 | 445.125 | High traffic areas |
| 83 | 493.125 | Maximum frequency |

### Regional Considerations
- **433 MHz ISM Band**: Channel 23 (433.125 MHz)
- **Europe**: 433.05-434.79 MHz allowed
- **US**: 433.05-434.79 MHz (Part 15)
- **Check local regulations** before selecting channels

## ⚡ Speed Configuration (SPED)

The SPED byte controls both UART baud rate and air data rate.

### SPED Byte Structure
```
Bit 7-5: UART Baud Rate
Bit 4-3: UART Parity  
Bit 2-0: Air Data Rate
```

### UART Baud Rates (Bits 7-5)

| Bits | Value | Baud Rate |
|------|-------|-----------|
| 000 | 0x00 | 1200 |
| 001 | 0x20 | 2400 |
| 010 | 0x40 | 4800 |
| 011 | 0x60 | 9600 |
| 100 | 0x80 | 19200 |
| 101 | 0xA0 | 38400 |
| 110 | 0xC0 | 57600 |
| 111 | 0xE0 | 115200 |

### UART Parity (Bits 4-3)

| Bits | Value | Parity |
|------|-------|--------|
| 00 | 0x00 | 8N1 (8 data, no parity, 1 stop) |
| 01 | 0x08 | 8O1 (8 data, odd parity, 1 stop) |
| 10 | 0x10 | 8E1 (8 data, even parity, 1 stop) |
| 11 | 0x18 | Reserved |

### Air Data Rates (Bits 2-0)

| Bits | Value | Air Rate (bps) | Range | Power |
|------|-------|---------------|-------|-------|
| 000 | 0x00 | 2400 | Maximum | Low |
| 001 | 0x01 | 2400 | Maximum | Low |
| 010 | 0x02 | 2400 | Maximum | Low |
| 011 | 0x03 | 4800 | Good | Medium |
| 100 | 0x04 | 4800 | Good | Medium |
| 101 | 0x05 | 9600 | Medium | Medium |
| 110 | 0x06 | 19200 | Short | High |
| 111 | 0x07 | 38400 | Shortest | Highest |

### Common SPED Configurations

```cpp
// 9600 baud, 8N1, 2.4k air rate (maximum range)
config.SPED = 0x60; // 0110 00 00

// 9600 baud, 8N1, 19.2k air rate (fast, shorter range)  
config.SPED = 0x66; // 0110 00 10

// 115200 baud, 8N1, 38.4k air rate (fastest, shortest range)
config.SPED = 0xE7; // 1110 00 11
```

## 🔧 Option Configuration (OPTION)

The OPTION byte controls various device behaviors.

### OPTION Byte Structure
```
Bit 7: Transmission Power
Bit 6: FEC (Forward Error Correction)
Bit 5: Wireless Wake-up Time
Bit 4-3: IO Drive Mode
Bit 2: LBT (Listen Before Talk)
Bit 1-0: WOR Period
```

### Transmission Power (Bit 7)

| Bit | Value | Power | Range |
|-----|-------|-------|-------|
| 0 | 0x00 | 22 dBm (158 mW) | Maximum |
| 1 | 0x80 | 17 dBm (50 mW) | Reduced |

### FEC - Forward Error Correction (Bit 6)

| Bit | Value | FEC | Description |
|-----|-------|-----|-------------|
| 0 | 0x00 | OFF | No error correction, faster |
| 1 | 0x40 | ON | Error correction, more reliable |

### Wireless Wake-up Time (Bit 5)

| Bit | Value | Wake Time | Use Case |
|-----|-------|-----------|----------|
| 0 | 0x00 | 250ms | Normal applications |
| 1 | 0x20 | 500ms | Power-sensitive applications |

### IO Drive Mode (Bits 4-3)

| Bits | Value | Mode | Description |
|------|-------|------|-------------|
| 00 | 0x00 | TXD, AUX push-pull | Standard output |
| 01 | 0x08 | TXD, AUX open-collector | External pull-up needed |
| 10 | 0x10 | Reserved | - |
| 11 | 0x18 | Reserved | - |

### LBT - Listen Before Talk (Bit 2)

| Bit | Value | LBT | Description |
|-----|-------|-----|-------------|
| 0 | 0x00 | OFF | Transmit immediately |
| 1 | 0x04 | ON | Check channel before transmit |

### WOR Period (Bits 1-0)

| Bits | Value | Period | Power Consumption |
|------|-------|--------|------------------|
| 00 | 0x00 | 500ms | Highest |
| 01 | 0x01 | 1000ms | High |
| 10 | 0x02 | 1500ms | Low |
| 11 | 0x03 | 2000ms | Lowest |

### Common OPTION Configurations

```cpp
// Maximum performance: High power, FEC on, fast wake
config.OPTION = 0x40; // 0100 00 00

// Balanced: Medium power, FEC on, LBT on
config.OPTION = 0x44; // 0100 01 00

// Power saving: Low power, FEC off, slow wake, long WOR
config.OPTION = 0x83; // 1000 00 11
```

## 🛠️ Configuration Examples

### High-Performance Configuration
```cpp
Configuration config;
config.ADDH = 0x00;      // Address 0x0001
config.ADDL = 0x01;
config.SPED = 0x60;      // 9600 baud, 2.4k air (max range)
config.CHAN = 23;        // 433.125 MHz
config.OPTION = 0x40;    // High power, FEC on
```

### Power-Saving Configuration
```cpp
Configuration config;
config.ADDH = 0x00;      // Address 0x0002  
config.ADDL = 0x02;
config.SPED = 0x66;      // 9600 baud, 19.2k air (faster)
config.CHAN = 23;        // 433.125 MHz
config.OPTION = 0x83;    // Low power, long WOR period
```

### High-Speed Configuration
```cpp
Configuration config;
config.ADDH = 0x00;      // Address 0x0003
config.ADDL = 0x03;
config.SPED = 0xE7;      // 115200 baud, 38.4k air (fastest)
config.CHAN = 35;        // 445.125 MHz
config.OPTION = 0x44;    // Medium power, FEC on, LBT on
```

## 📝 Reading and Writing Configuration

### Reading Current Configuration
```cpp
ResponseStructContainer response = e220ttl.getConfiguration();
if (response.status.code == E220_SUCCESS) {
    Configuration* config = (Configuration*)response.data;
    
    Serial.println("Current Configuration:");
    Serial.printf("Address: 0x%02X%02X\n", config->ADDH, config->ADDL);
    Serial.printf("Channel: %d\n", config->CHAN);
    Serial.printf("SPED: 0x%02X\n", config->SPED);
    Serial.printf("OPTION: 0x%02X\n", config->OPTION);
}
response.close(); // Free memory
```

### Writing New Configuration
```cpp
// Read current configuration
ResponseStructContainer response = e220ttl.getConfiguration();
Configuration* config = (Configuration*)response.data;

// Modify parameters
config->ADDH = 0x00;
config->ADDL = 0x05;
config->CHAN = 23;
config->SPED = 0x60;    // 9600 baud, 2.4k air
config->OPTION = 0x40;  // High power, FEC on

// Write back to device
ResponseStatus status = e220ttl.setConfiguration(*config, WRITE_CFG_PWR_DWN_SAVE);
response.close();

if (status.code == E220_SUCCESS) {
    Serial.println("Configuration updated successfully!");
} else {
    Serial.println("Configuration update failed!");
}
```

### Save Types
- **WRITE_CFG_PWR_DWN_SAVE**: Save to EEPROM (persistent)
- **WRITE_CFG_PWR_DWN_LOSE**: Temporary (lost on restart)

## ⚠️ Configuration Best Practices

### 1. Device Compatibility
- All communicating devices must have compatible settings
- Same channel required for communication
- Compatible air data rates recommended

### 2. Range vs Speed Trade-offs
- Lower air data rates = longer range
- Higher air data rates = faster transmission
- FEC improves reliability but reduces throughput

### 3. Power Management
- Use lowest power setting that meets range requirements
- Enable WOR for battery-powered devices
- Consider LBT in congested environments

### 4. Channel Selection
- Avoid channels with local interference
- Use spectrum analyzer if available
- Test multiple channels for best performance

### 5. Configuration Verification
```cpp
// Always verify configuration was set correctly
ResponseStructContainer newConfig = e220ttl.getConfiguration();
Configuration* verify = (Configuration*)newConfig.data;

if (verify->CHAN == expectedChannel) {
    Serial.println("Configuration verified!");
} else {
    Serial.println("Configuration mismatch!");
}
newConfig.close();
```

## 🔗 Related Documentation

- [API Reference](api-reference.md) - Method documentation
- [Examples](examples.md) - Working configuration examples
- [Troubleshooting](troubleshooting.md) - Configuration issues
- [Performance](performance.md) - Optimization guidelines
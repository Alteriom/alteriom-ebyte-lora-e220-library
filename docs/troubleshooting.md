# Troubleshooting

## Common Issues and Solutions

### Installation Problems

#### Library Not Found
**Problem**: Arduino IDE can't find the EByte LoRa E220 library

**Solutions**:
- Verify library is installed in correct directory: `Documents/Arduino/libraries/`
- Restart Arduino IDE after installation
- Check library name: should be `EByte_LoRa_E220_Series_Library`

#### Compilation Errors
**Problem**: Code won't compile with errors like "class not found"

**Solutions**:
- Update Arduino IDE to latest version (2.0+ recommended)
- Verify correct board is selected in Tools → Board
- Check that all required headers are included

### Hardware Connection Issues

#### No Response from Module
**Problem**: Module doesn't respond to configuration commands

**Solutions**:
1. **Check Wiring**:
   ```
   E220 → Microcontroller
   VCC  → 3.3V (NOT 5V)
   GND  → GND
   TX   → Pin 2 (or your chosen RX pin)
   RX   → Pin 3 (or your chosen TX pin)
   M0   → Pin 4 (or your chosen M0 pin)
   M1   → Pin 5 (or your chosen M1 pin)
   AUX  → Pin 6 (or your chosen AUX pin)
   ```

2. **Voltage Issues**:
   - E220 requires 3.3V power
   - Use voltage divider for 5V Arduino boards
   - Check power supply current capability (≥250mA)

3. **Serial Communication**:
   - Try different baud rates (9600, 115200)
   - Check TX/RX pins aren't swapped
   - Verify software serial configuration

#### AUX Pin Always High/Low
**Problem**: AUX pin doesn't change state properly

**Solutions**:
- Check AUX pin connection
- Add pull-up resistor (10kΩ) if needed
- Verify pin isn't used by other components
- Check for loose connections

### Communication Problems

#### Messages Not Being Received
**Problem**: Sender appears to transmit but receiver gets nothing

**Solutions**:
1. **Configuration Mismatch**:
   - Verify both modules have same channel
   - Check address configuration matches
   - Ensure same air data rate settings

2. **Range Issues**:
   - Start with modules close together (1-2 meters)
   - Check antenna connections
   - Verify clear line of sight

3. **Timing Issues**:
   - Add delays between transmissions
   - Check for proper AUX pin handling
   - Verify WOR timing if using sleep mode

#### Garbled or Partial Messages
**Problem**: Receiving corrupted or incomplete data

**Solutions**:
- Check serial baud rate settings
- Verify buffer sizes are adequate
- Add error checking to your code
- Check for interference (WiFi, other LoRa devices)

### Configuration Issues

#### Can't Read Configuration
**Problem**: `getConfiguration()` returns error or timeout

**Solutions**:
1. **Mode Settings**:
   ```cpp
   // Ensure module is in configuration mode
   // M0=HIGH, M1=HIGH for configuration
   ```

2. **Timing**:
   - Add delay after setting M0/M1 pins
   - Wait for AUX pin to go HIGH before sending commands
   - Use longer timeouts for slow modules

#### Configuration Changes Don't Persist
**Problem**: Settings revert after power cycle

**Solutions**:
- Use `setConfiguration()` with `WRITE_CFG_PWR_DWN_SAVE` option
- Verify configuration write was successful
- Check for power supply instability

### Performance Issues

#### Short Range
**Problem**: Communication range much less than expected

**Solutions**:
- Use appropriate antenna (gain matched to power setting)
- Check for interference sources
- Increase transmission power setting
- Verify clear line of sight
- Check ground plane for antenna

#### High Power Consumption
**Problem**: Module drains battery quickly

**Solutions**:
- Enable WOR (Wake on Radio) mode
- Reduce transmission power to minimum needed
- Use fixed transmission mode to reduce overhead
- Implement proper sleep modes

## Advanced Diagnostics

### Debug Output
Enable debug output to see detailed communication:

```cpp
// Uncomment in LoRa_E220.h
#define E220_DEBUG

// Or add to your sketch
#define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
#define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)
```

### RSSI Monitoring
Check signal strength:

```cpp
ResponseStructContainer c = e220ttl.getRSSI();
if (c.status.code == 1) {
    Serial.print("RSSI: ");
    Serial.println(c.data.RSSIAmbientNoise);
}
c.close();
```

### Configuration Dump
Print complete configuration:

```cpp
ResponseStructContainer c = e220ttl.getConfiguration();
if (c.status.code == 1) {
    Configuration configuration = c.data;
    printParameters(configuration);
}
c.close();
```

## Getting Help

If these solutions don't resolve your issue:

1. Check the [Examples Library](examples.md) for working code
2. Review the [API Reference](api-reference.md) for correct function usage
3. Visit the [GitHub Issues](https://github.com/xreef/EByte_LoRa_E220_Series_Library/issues) page
4. Post detailed information including:
   - Hardware used (Arduino model, E220 variant)
   - Wiring diagram
   - Complete code that reproduces the issue
   - Serial output with debug enabled

Remember: Most issues are related to wiring, power supply, or configuration mismatches. Start with the basics!
/**
 * @file LoRa_E220.h
 * @brief EBYTE LoRa E220 Series Library - Alteriom Fork
 * 
 * This library provides a comprehensive interface for EByte LoRa E220 Series devices
 * based on the LLCC68 chipset, offering long-range communication capabilities 
 * (5-10km range) for Arduino, ESP32, ESP8266, STM32, and Raspberry Pi Pico platforms.
 *
 * @author Renzo Mischianti (Original)
 * @author Alteriom (Fork maintainer with CI/CD enhancements)
 * @version 1.1.6
 * 
 * VERSION: 1.1.6
 * 
 * @see Original: https://www.mischianti.org
 * @see Fork: https://github.com/Alteriom/EByte_LoRa_E220_Series_Library
 *
 * @note The MIT License (MIT)
 *
 * Copyright (c) 2022 Renzo Mischianti www.mischianti.org All right reserved.
 * Copyright (c) 2024 Alteriom - Enhancements and CI/CD improvements
 *
 * You may copy, alter and reuse this code in any way you like, but please leave
 * reference to www.mischianti.org in your comments if you redistribute this code.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef LoRa_E220_h
#define LoRa_E220_h

/**
 * @brief Platform detection and serial interface configuration
 * 
 * These preprocessor directives enable platform-specific features:
 * - Software Serial: Activated for platforms without multiple hardware UART
 * - Selectable Hardware Serial Pins: Available on ESP32 platform
 * 
 * @note Software Serial is automatically enabled for Arduino Uno, Nano, Pro Mini
 * @note ESP32 supports configurable RX/TX pins for Hardware Serial
 */
#if !defined(ARDUINO_ARCH_STM32) && !defined(ESP32) && !defined(ARDUINO_ARCH_SAMD) && !defined(ARDUINO_ARCH_MBED) && !defined(__STM32F1__) && !defined(__STM32F4__)
	#define ACTIVATE_SOFTWARE_SERIAL
#endif
#if defined(ESP32)
	#define HARDWARE_SERIAL_SELECTABLE_PIN
#endif

#ifdef ACTIVATE_SOFTWARE_SERIAL
	#include <SoftwareSerial.h>
#endif

#include <includes/statesNaming.h>

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/**
 * @brief Maximum size for transmission packets
 * @note The E220 series supports up to 200 bytes per transmission
 * @warning Attempting to send larger packets will result in ERR_E220_PACKET_TOO_BIG
 */
#define MAX_SIZE_TX_PACKET 200

/**
 * @brief Debug output configuration
 * 
 * Uncomment LoRa_E220_DEBUG to enable detailed debugging information.
 * Debug messages will be output to the serial interface defined by DEBUG_PRINTER.
 * 
 * @note Debug output may impact performance and should be disabled in production
 * @example 
 * @code
 * #define LoRa_E220_DEBUG  // Enable debug output
 * @endcode
 */
// Uncomment to enable printing out nice debug messages.
//#define LoRa_E220_DEBUG

/**
 * @brief Debug output destination
 * @note By default, debug messages are sent to the primary Serial interface
 */
// Define where debug output will be printed.
#define DEBUG_PRINTER Serial

/**
 * @brief Debug printing macros
 * 
 * These macros provide conditional debug output:
 * - DEBUG_PRINT: Print without newline
 * - DEBUG_PRINTLN: Print with newline
 * 
 * @note Macros are conditionally compiled based on LoRa_E220_DEBUG definition
 */
// Setup debug printing macros.
#ifdef LoRa_E220_DEBUG
	#define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
	#define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
	#define DEBUG_PRINT(...) {}
	#define DEBUG_PRINTLN(...) {}
#endif

/**
 * @brief Operating mode types for the E220 LoRa module
 * 
 * The E220 module operates in different modes controlled by M0 and M1 pins:
 * - MODE_0: Normal transmission mode (M0=0, M1=0)
 * - MODE_1: Wake-on-Radio (WOR) transmitter mode (M0=1, M1=0)  
 * - MODE_2: Wake-on-Radio (WOR) receiver/power saving mode (M0=0, M1=1)
 * - MODE_3: Configuration/programming mode (M0=1, M1=1)
 * 
 * @note Mode changes require physical control of M0/M1 pins or software commands
 * @warning Configuration mode (MODE_3) requires 9600 bps UART speed
 */
enum MODE_TYPE {
	MODE_0_NORMAL 			= 0,  ///< Normal transmission mode - default operating mode
	MODE_0_TRANSMISSION 	= 0,  ///< Alias for normal transmission mode
	MODE_1_WOR_TRANSMITTER 	= 1,  ///< Wake-on-Radio transmitter mode for power-efficient communication
	MODE_1_WOR				= 1,  ///< Alias for WOR mode
	MODE_2_WOR_RECEIVER 	= 2,  ///< Wake-on-Radio receiver mode with automatic wake-up
	MODE_2_POWER_SAVING 	= 2,  ///< Alias for power saving mode
	MODE_3_CONFIGURATION 	= 3,  ///< Configuration mode for parameter changes
	MODE_3_PROGRAM 			= 3,  ///< Alias for programming mode
	MODE_3_SLEEP 			= 3,  ///< Alias for sleep mode
	MODE_INIT 				= 0xFF ///< Internal initialization state
};

/**
 * @brief Programming commands for device configuration
 * 
 * These commands control how configuration parameters are stored:
 * - WRITE_CFG_PWR_DWN_SAVE: Save configuration permanently (survives power cycle)
 * - READ_CONFIGURATION: Read current configuration from device
 * - WRITE_CFG_PWR_DWN_LOSE: Temporary configuration (lost on power cycle)
 * 
 * @note WRITE_CFG_PWR_DWN_SAVE stores settings in EEPROM for permanent retention
 * @warning Invalid commands will return WRONG_FORMAT error
 */
enum PROGRAM_COMMAND {
	WRITE_CFG_PWR_DWN_SAVE 	= 0xC0,  ///< Write configuration with power-down save to EEPROM
	READ_CONFIGURATION 		= 0xC1,  ///< Read current device configuration
	WRITE_CFG_PWR_DWN_LOSE 	= 0xC2,  ///< Write configuration temporarily (lost on power cycle)
	WRONG_FORMAT 			= 0xFF,  ///< Invalid command format error code
	RETURNED_COMMAND 		= 0xC1,  ///< Expected response command for configuration reads
	SPECIAL_WIFI_CONF_COMMAND = 0xCF ///< Special WiFi configuration command (advanced usage)
};

/**
 * @brief Register addresses for device configuration parameters
 * 
 * These addresses define the memory locations of configuration parameters:
 * - REG_ADDRESS_CFG: Base configuration address (0x00)
 * - REG_ADDRESS_SPED: Speed settings (UART baud rate, air data rate, parity)
 * - REG_ADDRESS_TRANS_MODE: Transmission mode settings (fixed/transparent, RSSI, LBT)
 * - REG_ADDRESS_CHANNEL: Operating frequency channel (0-255)
 * - REG_ADDRESS_OPTION: Additional options (transmission power, sub-packet size)
 * - REG_ADDRESS_CRYPT: Encryption key settings
 * - REG_ADDRESS_PID: Product identification
 * 
 * @note These addresses are used internally for register access
 * @see Configuration structure for parameter organization
 */
enum REGISTER_ADDRESS {
	REG_ADDRESS_CFG			= 0x00,  ///< Base configuration register address
	REG_ADDRESS_SPED 		= 0x02,  ///< Speed configuration register (UART + Air data rate)
	REG_ADDRESS_TRANS_MODE 	= 0x03,  ///< Transmission mode register (Fixed/Transparent, RSSI, LBT)
	REG_ADDRESS_CHANNEL 	= 0x04,  ///< Channel register (operating frequency)
	REG_ADDRESS_OPTION	 	= 0x05,  ///< Option register (power, sub-packet settings)
	REG_ADDRESS_CRYPT	 	= 0x06,  ///< Encryption register (security key)
	REG_ADDRESS_PID		 	= 0x08   ///< Product identification register
};

/**
 * @brief Packet length definitions for configuration parameters
 * 
 * These constants define the expected data lengths for various configuration operations:
 * - PL_CONFIGURATION: Complete configuration packet size (8 bytes)
 * - PL_SPED: Speed configuration size (1 byte)
 * - PL_OPTION: Option configuration size (1 byte)
 * - PL_TRANSMISSION_MODE: Transmission mode size (1 byte)
 * - PL_CHANNEL: Channel configuration size (1 byte)
 * - PL_CRYPT: Encryption key size (2 bytes)
 * - PL_PID: Product ID size (3 bytes)
 * 
 * @note These lengths are used for packet validation and buffer sizing
 * @warning Incorrect packet lengths will result in communication errors
 */
enum PACKET_LENGHT {
	PL_CONFIGURATION 	= 0x08,  ///< Complete configuration packet length (8 bytes)
	
	PL_SPED				= 0x01,  ///< Speed configuration packet length (1 byte)
	PL_OPTION			= 0x01,  ///< Option configuration packet length (1 byte)
	PL_TRANSMISSION_MODE= 0x01,  ///< Transmission mode packet length (1 byte)
	PL_CHANNEL			= 0x01,  ///< Channel configuration packet length (1 byte)
	PL_CRYPT			= 0x02,  ///< Encryption key packet length (2 bytes)
	PL_PID				= 0x03   ///< Product identification packet length (3 bytes)
};

/**
 * @brief Packed structure alignment for binary compatibility
 * @note Ensures structures match the exact binary layout expected by E220 hardware
 */
#pragma pack(push, 1)

/**
 * @brief Speed configuration structure for UART and air interface settings
 * 
 * This structure defines the communication speed parameters:
 * - Air data rate: LoRa transmission speed (2.4kbps to 62.5kbps)
 * - UART parity: Serial communication parity (8N1, 8O1, 8E1)
 * - UART baud rate: Serial interface speed (1200bps to 115200bps)
 * 
 * @note Bit field layout matches E220 register format exactly
 * @warning Ensure UART baud rate matches the connected device configuration
 * 
 * @example Setting up speed configuration:
 * @code
 * Speed speed;
 * speed.airDataRate = AIR_DATA_RATE_010_24;    // 2.4kbps air rate
 * speed.uartParity = MODE_00_8N1;              // 8N1 parity
 * speed.uartBaudRate = UART_BPS_9600;          // 9600 bps UART
 * @endcode
 */
struct Speed {
	uint8_t airDataRate :3; ///< Air data rate (bits 0-2): LoRa transmission speed
	/**
	 * @brief Get human-readable air data rate description
	 * @return String describing the current air data rate setting
	 * @see getAirDataRateDescriptionByParams() for implementation details
	 */
	String getAirDataRateDescription() {
		return getAirDataRateDescriptionByParams(this->airDataRate);
	}

	uint8_t uartParity :2; ///< UART parity (bits 3-4): Serial communication parity settings
	/**
	 * @brief Get human-readable UART parity description
	 * @return String describing the current UART parity setting (e.g., "8N1", "8O1", "8E1")
	 * @see getUARTParityDescriptionByParams() for implementation details
	 */
	String getUARTParityDescription() {
		return getUARTParityDescriptionByParams(this->uartParity);
	}

	uint8_t uartBaudRate :3; ///< UART baud rate (bits 5-7): Serial interface speed
	/**
	 * @brief Get human-readable UART baud rate description
	 * @return String describing the current UART baud rate (e.g., "9600bps", "115200bps")
	 * @see getUARTBaudRateDescriptionByParams() for implementation details
	 */
	String getUARTBaudRateDescription() {
		return getUARTBaudRateDescriptionByParams(this->uartBaudRate);
	}
};

/**
 * @brief Transmission mode configuration structure
 * 
 * This structure controls advanced transmission parameters:
 * - WOR Period: Wake-on-Radio timing (500ms to 4000ms)
 * - LBT: Listen Before Talk collision avoidance
 * - Fixed Transmission: Addressing mode (transparent vs fixed)
 * - RSSI: Signal strength reporting
 * 
 * @note Bit field layout matches E220 register format exactly
 * @warning Changing these settings affects power consumption and compatibility
 * 
 * @example Configuring transmission mode:
 * @code
 * TransmissionMode mode;
 * mode.WORPeriod = WOR_2000_011;              // 2000ms WOR period
 * mode.enableLBT = LBT_DISABLED;              // Disable LBT
 * mode.fixedTransmission = FT_TRANSPARENT_TRANSMISSION; // Transparent mode
 * mode.enableRSSI = RSSI_ENABLED;             // Enable RSSI reporting
 * @endcode
 */
struct TransmissionMode {
	byte WORPeriod :3; ///< Wake-on-Radio period (bits 2,1,0): Power saving wake-up interval
	/**
	 * @brief Get human-readable WOR period description
	 * @return String describing the current WOR period setting (e.g., "2000ms")
	 * @see getWORPeriodByParams() for implementation details
	 */
	String getWORPeriodByParamsDescription() {
		return getWORPeriodByParams(this->WORPeriod);
	}

	byte reserved2 :1; ///< Reserved bit (bit 3): Must be 0

	byte enableLBT :1; ///< Listen Before Talk enable (bit 4): Collision avoidance feature
	/**
	 * @brief Get human-readable LBT enable description
	 * @return String describing LBT status ("Enabled" or "Disabled")
	 * @note LBT helps avoid transmission collisions in busy environments
	 * @see getLBTEnableByteByParams() for implementation details
	 */
	String getLBTEnableByteDescription() {
		return getLBTEnableByteByParams(this->enableLBT);
	}

	byte reserved :1; ///< Reserved bit (bit 5): Must be 0

	byte fixedTransmission :1; ///< Fixed transmission mode (bit 6): Addressing mode control
	/**
	 * @brief Get human-readable fixed transmission description
	 * @return String describing transmission mode ("Transparent" or "Fixed")
	 * @note Fixed mode uses first 3 bytes for addressing, transparent mode sends data directly
	 * @see getFixedTransmissionDescriptionByParams() for implementation details
	 */
	String getFixedTransmissionDescription() {
		return getFixedTransmissionDescriptionByParams(this->fixedTransmission);
	}

	byte enableRSSI :1; ///< RSSI enable (bit 7): Signal strength reporting
	/**
	 * @brief Get human-readable RSSI enable description
	 * @return String describing RSSI status ("Enabled" or "Disabled")
	 * @note When enabled, RSSI data is appended to received messages
	 * @see getRSSIEnableByteByParams() for implementation details
	 */
	String getRSSIEnableByteDescription() {
		return getRSSIEnableByteByParams(this->enableRSSI);
	}
};

/**
 * @brief Option configuration structure for advanced settings
 * 
 * This structure controls transmission power and packet handling:
 * - Transmission Power: RF output power level (10dBm to 22dBm/30dBm)
 * - RSSI Ambient Noise: Background noise level monitoring
 * - Sub-packet Setting: Maximum packet size (32 to 200 bytes)
 * 
 * @note Higher transmission power increases range but also power consumption
 * @warning Power levels depend on device variant (22dBm standard, 30dBm high-power)
 * 
 * @example Configuring device options:
 * @code
 * Option options;
 * options.transmissionPower = POWER_22;           // Maximum power (22dBm)
 * options.RSSIAmbientNoise = RSSI_AMBIENT_NOISE_ENABLED; // Monitor noise
 * options.subPacketSetting = SPS_200_00;          // 200-byte packets
 * @endcode
 */
struct Option {
	uint8_t transmissionPower :2; ///< Transmission power (bits 0-1): RF output power level
	/**
	 * @brief Get human-readable transmission power description
	 * @return String describing current power level (e.g., "22dBm", "17dBm")
	 * @note Actual power levels depend on device variant (E220-22 vs E220-30)
	 * @see getTransmissionPowerDescriptionByParams() for implementation details
	 */
	String getTransmissionPowerDescription() {
		return getTransmissionPowerDescriptionByParams(this->transmissionPower);
	}

	uint8_t reserved :3; ///< Reserved bits (bits 2-4): Must be 0

	uint8_t RSSIAmbientNoise :1; ///< RSSI ambient noise enable (bit 5): Background noise monitoring
	/**
	 * @brief Get human-readable RSSI ambient noise enable description
	 * @return String describing noise monitoring status ("Enabled" or "Disabled")
	 * @note Enables monitoring of background RF noise levels
	 * @see getRSSIAmbientNoiseEnableByParams() for implementation details
	 */
	String getRSSIAmbientNoiseEnable() {
		return getRSSIAmbientNoiseEnableByParams(this->RSSIAmbientNoise);
	}

	uint8_t subPacketSetting :2; ///< Sub-packet setting (bits 6-7): Maximum packet size configuration
	/**
	 * @brief Get human-readable sub-packet setting description
	 * @return String describing packet size limit (e.g., "200bytes", "128bytes")
	 * @note Smaller packets improve reliability but reduce throughput
	 * @see getSubPacketSettingByParams() for implementation details
	 */
	String getSubPacketSetting() {
		return getSubPacketSettingByParams(this->subPacketSetting);
	}
};

/**
 * @brief Encryption configuration structure
 * 
 * This structure holds the 16-bit encryption key for secure communication:
 * - CRYPT_H: High byte of encryption key
 * - CRYPT_L: Low byte of encryption key
 * 
 * @note When encryption is enabled, both devices must use the same key
 * @warning Default value (0x0000) means no encryption
 * 
 * @example Setting encryption key:
 * @code
 * Crypt encryption;
 * encryption.CRYPT_H = 0x12;  // High byte
 * encryption.CRYPT_L = 0x34;  // Low byte (key = 0x1234)
 * @endcode
 */
struct Crypt {
	byte CRYPT_H = 0; ///< Encryption key high byte
	byte CRYPT_L = 0; ///< Encryption key low byte
};

/**
 * @brief Complete device configuration structure
 * 
 * This structure contains all configurable parameters for the E220 device:
 * - Command header: Programming command and addressing
 * - Device address: High and low bytes for device identification
 * - Speed settings: UART and air interface configuration
 * - Options: Power, packet size, and feature settings
 * - Channel: Operating frequency selection
 * - Transmission mode: Advanced communication settings
 * - Encryption: Security key configuration
 * 
 * @note This structure maps directly to the device's internal configuration registers
 * @warning All fields must be properly initialized before writing to device
 * 
 * @example Complete configuration setup:
 * @code
 * Configuration config;
 * config.ADDH = 0x00;           // High address byte
 * config.ADDL = 0x01;           // Low address byte  
 * config.CHAN = 23;             // Channel 23 (433MHz + 23MHz)
 * config.SPED.uartBaudRate = UART_BPS_9600;
 * config.OPTION.transmissionPower = POWER_22;
 * @endcode
 */
struct Configuration {
	byte COMMAND = 0;         ///< Programming command byte
	byte STARTING_ADDRESS = 0; ///< Starting register address for configuration
	byte LENGHT = 0;          ///< Configuration data length

	byte ADDH = 0; ///< Device address high byte (0x00-0xFF)
	byte ADDL = 0; ///< Device address low byte (0x00-0xFF)

	struct Speed SPED;   ///< Speed configuration (UART + air interface)
	struct Option OPTION; ///< Device options (power, packet size, features)

	byte CHAN = 0; ///< Operating channel (0-255, frequency = base + channel)
	/**
	 * @brief Get human-readable channel description with frequency
	 * @return String describing current channel and calculated frequency
	 * @note Frequency = OPERATING_FREQUENCY + CHAN (e.g., 410MHz + 23 = 433MHz)
	 * @see OPERATING_FREQUENCY definition for base frequency
	 */
	String getChannelDescription() {
		return String(this->CHAN + OPERATING_FREQUENCY) + F("MHz");
	}

	struct TransmissionMode TRANSMISSION_MODE; ///< Transmission mode settings

	struct Crypt CRYPT; ///< Encryption configuration
};

/**
 * @brief Module information structure for device identification
 * 
 * This structure contains hardware and firmware information about the device:
 * - Model: Device model identifier
 * - Version: Firmware version
 * - Features: Supported feature flags
 * 
 * @note This information is read-only and set by the manufacturer
 * @see getModuleInformation() to retrieve this data from device
 * 
 * @example Reading module information:
 * @code
 * ResponseStructContainer response = e220.getModuleInformation();
 * if (response.status.code == E220_SUCCESS) {
 *     ModuleInformation* info = (ModuleInformation*)response.data;
 *     Serial.print("Model: "); Serial.println(info->model);
 *     Serial.print("Version: "); Serial.println(info->version);
 * }
 * response.close();
 * @endcode
 */
struct ModuleInformation {
	byte COMMAND = 0;         ///< Command response header
	byte STARTING_ADDRESS = 0; ///< Starting address of information
	byte LENGHT = 0;          ///< Information data length

	byte model = 0;    ///< Device model identifier (device-specific)
	byte version = 0;  ///< Firmware version number
	byte features = 0; ///< Supported features bit flags
};

/**
 * @brief Response status structure for operation result reporting
 * 
 * This structure encapsulates the result of library operations:
 * - Code: Status code indicating success or specific error type
 * - Description: Human-readable explanation of the status
 * 
 * @note Always check the status code before using operation results
 * @see Status enum for complete list of possible status codes
 * 
 * @example Checking operation status:
 * @code
 * ResponseStatus status = e220.setConfiguration(config);
 * if (status.code == E220_SUCCESS) {
 *     Serial.println("Configuration updated successfully");
 * } else {
 *     Serial.print("Error: ");
 *     Serial.println(status.getResponseDescription());
 * }
 * @endcode
 */
struct ResponseStatus {
	Status code; ///< Operation result code (success or error type)
	/**
	 * @brief Get human-readable status description
	 * @return String explaining the current status code
	 * @note Provides detailed error messages for troubleshooting
	 * @see getResponseDescriptionByParams() for implementation details
	 */
	String getResponseDescription() {
		return getResponseDescriptionByParams(this->code);
	}
};

/**
 * @brief Response container for structured data with metadata
 * 
 * This structure provides a generic container for operation results that include:
 * - Data: Pointer to the actual response data (must be cast to appropriate type)
 * - RSSI: Signal strength measurement (when available)
 * - Status: Operation result and error information
 * 
 * @warning Always call close() to free allocated memory after use
 * @note RSSI value is only valid when RSSI reporting is enabled
 * 
 * @example Using response container:
 * @code
 * ResponseStructContainer response = e220.getConfiguration();
 * if (response.status.code == E220_SUCCESS) {
 *     Configuration* config = (Configuration*)response.data;
 *     // Use configuration data...
 *     Serial.print("RSSI: "); Serial.println(response.rssi);
 * }
 * response.close(); // Always free memory!
 * @endcode
 */
struct ResponseStructContainer {
	void *data;           ///< Pointer to response data (cast to appropriate struct type)
	byte rssi;            ///< Received Signal Strength Indicator (dBm)
	ResponseStatus status; ///< Operation status and error information
	
	/**
	 * @brief Free allocated memory for response data
	 * @warning Must be called after using the response to prevent memory leaks
	 * @note Sets data pointer to null after freeing
	 */
	void close() {
		free(this->data);
	}
};

/**
 * @brief Response container for string data with metadata
 * 
 * This structure provides a container for text-based responses:
 * - Data: String containing the response text
 * - RSSI: Signal strength measurement (when available)
 * - Status: Operation result and error information
 * 
 * @note No explicit cleanup required as String handles its own memory
 * @note RSSI value is only valid when RSSI reporting is enabled
 * 
 * @example Using string response container:
 * @code
 * ResponseContainer response = e220.receiveMessage();
 * if (response.status.code == E220_SUCCESS) {
 *     Serial.print("Received: "); Serial.println(response.data);
 *     Serial.print("RSSI: "); Serial.println(response.rssi);
 * }
 * @endcode
 */
struct ResponseContainer {
	String data;          ///< Response data as String
	byte rssi;            ///< Received Signal Strength Indicator (dBm)
	ResponseStatus status; ///< Operation status and error information
};

/**
 * @brief Configuration message structure for special WiFi configuration
 * 
 * This structure is used for advanced WiFi configuration commands:
 * - Special command bytes: Identify this as a configuration message
 * - Message payload: Variable-length configuration data
 * 
 * @note This is an advanced feature for specific use cases
 * @warning Incorrect usage may affect device functionality
 * 
 * @example WiFi configuration message:
 * @code
 * ConfigurationMessage wifiMsg;
 * wifiMsg.specialCommand1 = 0xCF;
 * wifiMsg.specialCommand2 = 0xCF;
 * // Configure message payload as needed
 * @endcode
 */
struct ConfigurationMessage
{
	byte specialCommand1 = 0xCF; ///< First special command identifier
	byte specialCommand2 = 0xCF; ///< Second special command identifier

	unsigned char message[]; ///< Variable-length message payload
};

/**
 * @brief Restore normal structure packing
 * @note Ends the packed structure section
 */
#pragma pack(pop)

/**
 * @brief Main LoRa E220 device interface class
 * 
 * This class provides a comprehensive interface for EByte LoRa E220 Series devices
 * with support for multiple platforms and communication interfaces:
 * 
 * **Supported Platforms:**
 * - Arduino (Uno, Nano, Pro Mini) - Software Serial
 * - ESP32/ESP8266 - Hardware Serial with configurable pins
 * - STM32 - Hardware Serial
 * - Raspberry Pi Pico - Hardware Serial
 * - SAMD - Hardware Serial
 * 
 * **Communication Features:**
 * - Transparent transmission (direct data exchange)
 * - Fixed transmission (addressed messaging)
 * - Broadcast messaging
 * - Wake-on-Radio (WOR) power saving
 * - RSSI signal strength monitoring
 * - Configuration management
 * 
 * **Hardware Requirements:**
 * - VCC: 3.3V or 5V (with voltage divider for 3.3V devices)
 * - TX/RX: Serial communication pins
 * - AUX: Optional status pin for advanced features
 * - M0/M1: Optional mode control pins
 * 
 * @note The device operates in different modes controlled by M0/M1 pins:
 *       - Normal (M0=0, M1=0): Standard communication
 *       - WOR Transmitter (M0=1, M1=0): Power-efficient sending
 *       - WOR Receiver (M0=0, M1=1): Power-efficient receiving
 *       - Configuration (M0=1, M1=1): Parameter setup mode
 * 
 * @warning Always check response status codes before using operation results
 * @warning Configuration mode requires 9600 bps UART speed regardless of other settings
 * 
 * @example Basic usage:
 * @code
 * #include "LoRa_E220.h"
 * 
 * // Hardware Serial setup
 * LoRa_E220 e220ttl(&Serial2, UART_BPS_RATE_9600);
 * 
 * void setup() {
 *     Serial.begin(9600);
 *     e220ttl.begin();
 *     
 *     // Send a message
 *     ResponseStatus rs = e220ttl.sendMessage("Hello LoRa!");
 *     if (rs.code == E220_SUCCESS) {
 *         Serial.println("Message sent successfully");
 *     }
 * }
 * @endcode
 */
class LoRa_E220 {
	public:
/**
 * @name Software Serial Constructors
 * @brief Constructors for platforms using Software Serial communication
 * 
 * These constructors are available on platforms that support Software Serial
 * (Arduino Uno, Nano, Pro Mini, etc.). They create a virtual serial interface
 * using digital pins for TX/RX communication.
 * 
 * @note Software Serial has limitations: lower baud rates, potential timing issues
 * @note Use Hardware Serial when available for better performance
 * @{
 */
#ifdef ACTIVATE_SOFTWARE_SERIAL
		/**
		 * @brief Constructor with Software Serial TX/RX pins
		 * @param txE220pin Digital pin number for TX to E220 (Arduino TX -> E220 RX)
		 * @param rxE220pin Digital pin number for RX from E220 (Arduino RX <- E220 TX)
		 * @param bpsRate UART baud rate (default: 9600 bps)
		 * 
		 * @note This is the minimal setup for Software Serial platforms
		 * @warning Pin numbers must be suitable for Software Serial (check platform docs)
		 * 
		 * @example Basic Software Serial setup:
		 * @code
		 * LoRa_E220 e220ttl(2, 3, UART_BPS_RATE_9600); // TX=pin2, RX=pin3
		 * @endcode
		 */
		LoRa_E220(byte txE220pin, byte rxE220pin, UART_BPS_RATE bpsRate = UART_BPS_RATE_9600);
		
		/**
		 * @brief Constructor with Software Serial and AUX pin
		 * @param txE220pin Digital pin number for TX to E220
		 * @param rxE220pin Digital pin number for RX from E220  
		 * @param auxPin Digital pin number for AUX status monitoring
		 * @param bpsRate UART baud rate (default: 9600 bps)
		 * 
		 * @note AUX pin provides status information for reliable communication
		 * @note AUX goes HIGH when device is ready, LOW when busy
		 * 
		 * @example Software Serial with AUX monitoring:
		 * @code
		 * LoRa_E220 e220ttl(2, 3, 4, UART_BPS_RATE_9600); // TX=2, RX=3, AUX=4
		 * @endcode
		 */
		LoRa_E220(byte txE220pin, byte rxE220pin, byte auxPin, UART_BPS_RATE bpsRate = UART_BPS_RATE_9600);
		
		/**
		 * @brief Constructor with Software Serial and full pin control
		 * @param txE220pin Digital pin number for TX to E220
		 * @param rxE220pin Digital pin number for RX from E220
		 * @param auxPin Digital pin number for AUX status monitoring
		 * @param m0Pin Digital pin number for M0 mode control
		 * @param m1Pin Digital pin number for M1 mode control  
		 * @param bpsRate UART baud rate (default: 9600 bps)
		 * 
		 * @note M0/M1 pins allow software control of device operating mode
		 * @note This provides maximum flexibility for mode switching
		 * 
		 * @example Full pin control setup:
		 * @code
		 * LoRa_E220 e220ttl(2, 3, 4, 5, 6); // TX=2, RX=3, AUX=4, M0=5, M1=6
		 * @endcode
		 */
		LoRa_E220(byte txE220pin, byte rxE220pin, byte auxPin, byte m0Pin, byte m1Pin, UART_BPS_RATE bpsRate = UART_BPS_RATE_9600);
#endif
/** @} */ // End of Software Serial Constructors group

/**
 * @name Hardware Serial Constructors  
 * @brief Constructors for platforms using Hardware Serial communication
 * 
 * These constructors use the platform's built-in UART hardware for
 * communication, providing better performance and reliability than Software Serial.
 * Available on ESP32, STM32, Raspberry Pi Pico, and other advanced platforms.
 * 
 * @note Hardware Serial offers higher baud rates and better timing accuracy
 * @note Recommended for production use when available
 * @{
 */
		/**
		 * @brief Constructor with Hardware Serial interface
		 * @param serial Pointer to HardwareSerial object (e.g., &Serial1, &Serial2)
		 * @param bpsRate UART baud rate (default: 9600 bps)
		 * 
		 * @note Uses hardware UART for reliable high-speed communication  
		 * @note Serial pins are platform-specific (check board documentation)
		 * 
		 * @example Basic Hardware Serial setup:
		 * @code
		 * LoRa_E220 e220ttl(&Serial2, UART_BPS_RATE_9600);
		 * @endcode
		 */
		LoRa_E220(HardwareSerial* serial, UART_BPS_RATE bpsRate = UART_BPS_RATE_9600);
		
		/**
		 * @brief Constructor with Hardware Serial and AUX pin
		 * @param serial Pointer to HardwareSerial object
		 * @param auxPin Digital pin number for AUX status monitoring
		 * @param bpsRate UART baud rate (default: 9600 bps)
		 * 
		 * @note AUX pin enables reliable communication timing
		 * @note Monitor AUX for device ready status before sending commands
		 * 
		 * @example Hardware Serial with AUX:
		 * @code
		 * LoRa_E220 e220ttl(&Serial2, 4, UART_BPS_RATE_9600); // Serial2, AUX=pin4
		 * @endcode
		 */
		LoRa_E220(HardwareSerial* serial, byte auxPin, UART_BPS_RATE bpsRate = UART_BPS_RATE_9600);
		
		/**
		 * @brief Constructor with Hardware Serial and full pin control
		 * @param serial Pointer to HardwareSerial object
		 * @param auxPin Digital pin number for AUX status monitoring
		 * @param m0Pin Digital pin number for M0 mode control
		 * @param m1Pin Digital pin number for M1 mode control
		 * @param bpsRate UART baud rate (default: 9600 bps)
		 * 
		 * @note Provides software control over device operating modes
		 * @note Enables automatic mode switching for configuration and operation
		 * 
		 * @example Full Hardware Serial setup:
		 * @code
		 * LoRa_E220 e220ttl(&Serial2, 4, 5, 6); // Serial2, AUX=4, M0=5, M1=6
		 * @endcode
		 */
		LoRa_E220(HardwareSerial* serial, byte auxPin, byte m0Pin, byte m1Pin, UART_BPS_RATE bpsRate = UART_BPS_RATE_9600);
/** @} */ // End of Hardware Serial Constructors group

/**
 * @name ESP32 Hardware Serial Constructors
 * @brief ESP32-specific constructors with configurable pins
 * 
 * These constructors are available only on ESP32 platform and allow
 * configuration of custom TX/RX pins for Hardware Serial interfaces.
 * This provides maximum flexibility for pin assignment.
 * 
 * @note ESP32 supports pin remapping for Hardware Serial interfaces
 * @note Useful when default serial pins conflict with other peripherals
 * @{
 */
#ifdef HARDWARE_SERIAL_SELECTABLE_PIN
		/**
		 * @brief ESP32 constructor with custom TX/RX pins
		 * @param txE220pin GPIO pin number for TX to E220
		 * @param rxE220pin GPIO pin number for RX from E220
		 * @param serial Pointer to HardwareSerial object (Serial1, Serial2)
		 * @param bpsRate UART baud rate
		 * @param serialConfig Serial configuration (default: SERIAL_8N1)
		 * 
		 * @note ESP32 allows remapping of Hardware Serial pins
		 * @note Check ESP32 pin matrix for valid TX/RX pin combinations
		 * 
		 * @example ESP32 custom pins:
		 * @code
		 * LoRa_E220 e220ttl(17, 16, &Serial2, UART_BPS_RATE_9600); // TX=17, RX=16
		 * @endcode
		 */
		LoRa_E220(byte txE220pin, byte rxE220pin, HardwareSerial* serial, UART_BPS_RATE bpsRate, uint32_t serialConfig = SERIAL_8N1);
		
		/**
		 * @brief ESP32 constructor with custom pins and AUX
		 * @param txE220pin GPIO pin number for TX to E220
		 * @param rxE220pin GPIO pin number for RX from E220
		 * @param serial Pointer to HardwareSerial object
		 * @param auxPin GPIO pin number for AUX status monitoring
		 * @param bpsRate UART baud rate
		 * @param serialConfig Serial configuration (default: SERIAL_8N1)
		 * 
		 * @example ESP32 with AUX monitoring:
		 * @code
		 * LoRa_E220 e220ttl(17, 16, &Serial2, 4, UART_BPS_RATE_9600);
		 * @endcode
		 */
		LoRa_E220(byte txE220pin, byte rxE220pin, HardwareSerial* serial, byte auxPin, UART_BPS_RATE bpsRate, uint32_t serialConfig = SERIAL_8N1);
		
		/**
		 * @brief ESP32 constructor with full pin control
		 * @param txE220pin GPIO pin number for TX to E220
		 * @param rxE220pin GPIO pin number for RX from E220
		 * @param serial Pointer to HardwareSerial object
		 * @param auxPin GPIO pin number for AUX status monitoring
		 * @param m0Pin GPIO pin number for M0 mode control
		 * @param m1Pin GPIO pin number for M1 mode control
		 * @param bpsRate UART baud rate
		 * @param serialConfig Serial configuration (default: SERIAL_8N1)
		 * 
		 * @note Provides complete hardware control on ESP32 platform
		 * @note Enables automatic mode switching and status monitoring
		 * 
		 * @example ESP32 full control:
		 * @code
		 * LoRa_E220 e220ttl(17, 16, &Serial2, 4, 5, 6, UART_BPS_RATE_9600);
		 * @endcode
		 */
		LoRa_E220(byte txE220pin, byte rxE220pin, HardwareSerial* serial, byte auxPin, byte m0Pin, byte m1Pin, UART_BPS_RATE bpsRate, uint32_t serialConfig = SERIAL_8N1);
#endif
/** @} */ // End of ESP32 Hardware Serial Constructors group

/**
 * @name Software Serial Object Constructors
 * @brief Constructors accepting pre-configured SoftwareSerial objects
 * 
 * These constructors accept existing SoftwareSerial objects, allowing
 * for advanced configuration or shared usage of serial interfaces.
 * 
 * @note Useful when SoftwareSerial object needs special configuration
 * @note Allows sharing of serial interface between multiple components
 * @{
 */
#ifdef ACTIVATE_SOFTWARE_SERIAL
		/**
		 * @brief Constructor with pre-configured SoftwareSerial object
		 * @param serial Pointer to configured SoftwareSerial object
		 * @param bpsRate UART baud rate (default: 9600 bps)
		 * 
		 * @note SoftwareSerial object must be pre-configured with pins
		 * @note Useful for complex pin configurations or shared usage
		 * 
		 * @example Using pre-configured SoftwareSerial:
		 * @code
		 * SoftwareSerial mySerial(2, 3);
		 * LoRa_E220 e220ttl(&mySerial, UART_BPS_RATE_9600);
		 * @endcode
		 */
		LoRa_E220(SoftwareSerial* serial, UART_BPS_RATE bpsRate = UART_BPS_RATE_9600);
		
		/**
		 * @brief Constructor with SoftwareSerial object and AUX pin
		 * @param serial Pointer to configured SoftwareSerial object
		 * @param auxPin Digital pin number for AUX status monitoring
		 * @param bpsRate UART baud rate (default: 9600 bps)
		 * 
		 * @example SoftwareSerial with AUX:
		 * @code
		 * SoftwareSerial mySerial(2, 3);
		 * LoRa_E220 e220ttl(&mySerial, 4, UART_BPS_RATE_9600);
		 * @endcode
		 */
		LoRa_E220(SoftwareSerial* serial, byte auxPin, UART_BPS_RATE bpsRate = UART_BPS_RATE_9600);
		
		/**
		 * @brief Constructor with SoftwareSerial object and full pin control
		 * @param serial Pointer to configured SoftwareSerial object
		 * @param auxPin Digital pin number for AUX status monitoring
		 * @param m0Pin Digital pin number for M0 mode control
		 * @param m1Pin Digital pin number for M1 mode control
		 * @param bpsRate UART baud rate (default: 9600 bps)
		 * 
		 * @example Full SoftwareSerial control:
		 * @code
		 * SoftwareSerial mySerial(2, 3);
		 * LoRa_E220 e220ttl(&mySerial, 4, 5, 6, UART_BPS_RATE_9600);
		 * @endcode
		 */
		LoRa_E220(SoftwareSerial* serial, byte auxPin, byte m0Pin, byte m1Pin, UART_BPS_RATE bpsRate = UART_BPS_RATE_9600);
#endif
/** @} */ // End of Software Serial Object Constructors group

//		LoRa_E220(byte txE220pin, byte rxE220pin, UART_BPS_RATE bpsRate = UART_BPS_RATE_9600, MODE_TYPE mode = MODE_0_NORMAL);
//		LoRa_E220(HardwareSerial* serial = &Serial, UART_BPS_RATE bpsRate = UART_BPS_RATE_9600, MODE_TYPE mode = MODE_0_NORMAL);
//		LoRa_E220(SoftwareSerial* serial, UART_BPS_RATE bpsRate = UART_BPS_RATE_9600, MODE_TYPE mode = MODE_0_NORMAL);

/**
 * @name Device Initialization and Mode Control
 * @brief Methods for device setup and operating mode management
 * @{
 */
		/**
		 * @brief Initialize the LoRa E220 device
		 * @return true if initialization successful, false otherwise
		 * 
		 * This method initializes the serial communication and prepares the device
		 * for operation. Must be called before any other device operations.
		 * 
		 * @note Call this method in setup() function after Serial.begin()
		 * @note Automatically configures serial interface and checks device status
		 * 
		 * @example Device initialization:
		 * @code
		 * void setup() {
		 *     Serial.begin(9600);
		 *     if (e220ttl.begin()) {
		 *         Serial.println("E220 initialized successfully");
		 *     } else {
		 *         Serial.println("E220 initialization failed");
		 *     }
		 * }
		 * @endcode
		 */
		bool begin();
		
		/**
		 * @brief Set device operating mode
		 * @param mode Operating mode to set (MODE_0_NORMAL, MODE_1_WOR, etc.)
		 * @return Status code indicating success or failure
		 * 
		 * Controls the device operating mode via M0/M1 pins (if available) or software commands.
		 * Different modes provide various power consumption and functionality characteristics.
		 * 
		 * @note MODE_3_CONFIGURATION required for parameter changes
		 * @note Mode changes may take several milliseconds to complete
		 * @warning Configuration mode requires 9600 bps UART speed
		 * 
		 * @example Setting device mode:
		 * @code
		 * Status result = e220ttl.setMode(MODE_0_NORMAL);
		 * if (result == E220_SUCCESS) {
		 *     Serial.println("Mode set successfully");
		 * }
		 * @endcode
		 */
        Status setMode(MODE_TYPE mode);
        
        /**
         * @brief Get current device operating mode
         * @return Current operating mode
         * 
         * Returns the currently active operating mode of the device.
         * Useful for verifying mode changes or checking device state.
         * 
         * @note May not reflect actual hardware state if M0/M1 pins changed externally
         * @example Checking current mode:
         * @code
         * MODE_TYPE currentMode = e220ttl.getMode();
         * if (currentMode == MODE_0_NORMAL) {
         *     Serial.println("Device in normal mode");
         * }
         * @endcode
         */
        MODE_TYPE getMode();
/** @} */ // End of Device Initialization and Mode Control group

/**
 * @name Configuration Management
 * @brief Methods for reading and writing device configuration parameters
 * @{
 */
		/**
		 * @brief Read current device configuration
		 * @return ResponseStructContainer with Configuration data and status
		 * 
		 * Retrieves the complete configuration from the device including:
		 * - Device address (ADDH, ADDL)
		 * - Speed settings (UART and air data rates)
		 * - Channel selection
		 * - Transmission options (power, packet size)
		 * - Transmission mode settings
		 * - Encryption configuration
		 * 
		 * @note Device must be in configuration mode (MODE_3_CONFIGURATION)
		 * @note UART must be set to 9600 bps for configuration operations
		 * @warning Always call close() on the returned container to free memory
		 * 
		 * @example Reading configuration:
		 * @code
		 * ResponseStructContainer response = e220ttl.getConfiguration();
		 * if (response.status.code == E220_SUCCESS) {
		 *     Configuration* config = (Configuration*)response.data;
		 *     Serial.print("Channel: "); Serial.println(config->CHAN);
		 *     Serial.print("Address: "); Serial.print(config->ADDH); 
		 *     Serial.println(config->ADDL);
		 * } else {
		 *     Serial.println("Failed to read configuration");
		 * }
		 * response.close(); // Always free memory!
		 * @endcode
		 */
		ResponseStructContainer getConfiguration();
		
		/**
		 * @brief Write device configuration
		 * @param configuration Configuration structure with new parameters
		 * @param saveType Save method (WRITE_CFG_PWR_DWN_LOSE or WRITE_CFG_PWR_DWN_SAVE)
		 * @return ResponseStatus indicating success or failure
		 * 
		 * Updates the device configuration with new parameters. The saveType parameter
		 * controls whether settings are saved permanently or temporarily:
		 * - WRITE_CFG_PWR_DWN_SAVE: Permanent (survives power cycle)
		 * - WRITE_CFG_PWR_DWN_LOSE: Temporary (lost on power cycle)
		 * 
		 * @note Device must be in configuration mode (MODE_3_CONFIGURATION)
		 * @note Configuration changes may require device reset to take effect
		 * @warning Incorrect configuration can make device unresponsive
		 * 
		 * @example Writing configuration:
		 * @code
		 * Configuration newConfig;
		 * newConfig.ADDH = 0x00;
		 * newConfig.ADDL = 0x01;
		 * newConfig.CHAN = 23;
		 * newConfig.SPED.uartBaudRate = UART_BPS_9600;
		 * 
		 * ResponseStatus status = e220ttl.setConfiguration(newConfig, WRITE_CFG_PWR_DWN_SAVE);
		 * if (status.code == E220_SUCCESS) {
		 *     Serial.println("Configuration saved successfully");
		 * }
		 * @endcode
		 */
		ResponseStatus setConfiguration(Configuration configuration, PROGRAM_COMMAND saveType = WRITE_CFG_PWR_DWN_LOSE);
/** @} */ // End of Configuration Management group

/**
 * @name Device Information and Control
 * @brief Methods for device identification and control operations
 * @{
 */
		/**
		 * @brief Get device module information
		 * @return ResponseStructContainer with ModuleInformation data and status
		 * 
		 * Retrieves hardware and firmware information from the device:
		 * - Model identifier
		 * - Firmware version
		 * - Feature flags
		 * 
		 * @note Useful for device identification and compatibility checking
		 * @note Device must be in configuration mode for this operation
		 * @warning Always call close() on the returned container to free memory
		 * 
		 * @example Getting module information:
		 * @code
		 * ResponseStructContainer response = e220ttl.getModuleInformation();
		 * if (response.status.code == E220_SUCCESS) {
		 *     ModuleInformation* info = (ModuleInformation*)response.data;
		 *     Serial.print("Model: 0x"); Serial.println(info->model, HEX);
		 *     Serial.print("Version: "); Serial.println(info->version);
		 *     Serial.print("Features: 0x"); Serial.println(info->features, HEX);
		 * }
		 * response.close();
		 * @endcode
		 */
		ResponseStructContainer getModuleInformation();
		
		/**
		 * @brief Reset device to default settings
		 * @return ResponseStatus indicating success or failure
		 * 
		 * Performs a software reset of the device, restoring factory default
		 * configuration parameters. This can be useful for recovering from
		 * configuration errors or resetting to known state.
		 * 
		 * @note Device must be in configuration mode for reset operation
		 * @note Reset may take several seconds to complete
		 * @warning All custom configuration will be lost
		 * 
		 * @example Resetting device:
		 * @code
		 * ResponseStatus status = e220ttl.resetModule();
		 * if (status.code == E220_SUCCESS) {
		 *     Serial.println("Device reset to factory defaults");
		 * } else {
		 *     Serial.println("Reset failed: " + status.getResponseDescription());
		 * }
		 * @endcode
		 */
		ResponseStatus resetModule();
/** @} */ // End of Device Information and Control group

/**
 * @name Message Transmission
 * @brief Methods for sending data using various transmission modes
 * @{
 */
		/**
		 * @brief Send binary data message
		 * @param message Pointer to data buffer to send
		 * @param size Number of bytes to send (max 200 bytes)
		 * @return ResponseStatus indicating success or failure
		 * 
		 * Sends raw binary data using the current transmission mode (transparent or fixed).
		 * In transparent mode, data is sent directly. In fixed mode, first 3 bytes
		 * are used for addressing (ADDH, ADDL, CHAN).
		 * 
		 * @note Maximum packet size is 200 bytes (MAX_SIZE_TX_PACKET)
		 * @note Device must be in normal transmission mode (MODE_0_NORMAL)
		 * @warning Exceeding 200 bytes will result in ERR_E220_PACKET_TOO_BIG
		 * 
		 * @example Sending binary data:
		 * @code
		 * uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
		 * ResponseStatus status = e220ttl.sendMessage(data, sizeof(data));
		 * if (status.code == E220_SUCCESS) {
		 *     Serial.println("Binary data sent successfully");
		 * }
		 * @endcode
		 */
		ResponseStatus sendMessage(const void *message, const uint8_t size);

/**
 * @name Message Reception
 * @brief Methods for receiving data with various options
 * @{
 */
	    /**
	     * @brief Receive message until delimiter character
	     * @param delimiter Character to stop reading at (default: null terminator)
	     * @return ResponseContainer with received string and status
	     * 
	     * Reads incoming data until the specified delimiter character is encountered.
	     * Useful for receiving text messages with known terminators.
	     * 
	     * @note Default delimiter ('\0') reads until null terminator
	     * @note RSSI information may be included if enabled in configuration
	     * 
	     * @example Receiving until newline:
	     * @code
	     * ResponseContainer response = e220ttl.receiveMessageUntil('\n');
	     * if (response.status.code == E220_SUCCESS) {
	     *     Serial.print("Received: "); Serial.println(response.data);
	     * }
	     * @endcode
	     */
	    ResponseContainer receiveMessageUntil(char delimiter = '\0');
	    
		/**
		 * @brief Receive fixed-size binary message
		 * @param size Number of bytes to receive
		 * @return ResponseStructContainer with binary data and status
		 * 
		 * Receives exactly the specified number of bytes as binary data.
		 * Useful for receiving structured data or when message size is known.
		 * 
		 * @note Blocks until specified number of bytes received or timeout
		 * @warning Always call close() on returned container to free memory
		 * 
		 * @example Receiving binary data:
		 * @code
		 * ResponseStructContainer response = e220ttl.receiveMessage(10);
		 * if (response.status.code == E220_SUCCESS) {
		 *     uint8_t* data = (uint8_t*)response.data;
		 *     // Process binary data...
		 * }
		 * response.close();
		 * @endcode
		 */
		ResponseStructContainer receiveMessage(const uint8_t size);
		
		/**
		 * @brief Receive fixed-size message with RSSI
		 * @param size Number of bytes to receive
		 * @return ResponseStructContainer with binary data, RSSI, and status
		 * 
		 * Receives exactly the specified number of bytes with signal strength information.
		 * RSSI value provides information about signal quality.
		 * 
		 * @note RSSI must be enabled in device configuration
		 * @note RSSI value is in dBm (typically -30 to -120)
		 * @warning Always call close() on returned container to free memory
		 * 
		 * @example Receiving with signal strength:
		 * @code
		 * ResponseStructContainer response = e220ttl.receiveMessageRSSI(10);
		 * if (response.status.code == E220_SUCCESS) {
		 *     Serial.print("RSSI: "); Serial.print(response.rssi); Serial.println(" dBm");
		 *     uint8_t* data = (uint8_t*)response.data;
		 *     // Process data...
		 * }
		 * response.close();
		 * @endcode
		 */
		ResponseStructContainer receiveMessageRSSI(const uint8_t size);
	        
        /**
         * @brief Receive complete message with optional RSSI
         * @param size Number of bytes to receive
         * @param enableRSSI Whether to include RSSI information
         * @return ResponseStructContainer with data and optional RSSI
         * 
         * Receives a complete message with optional signal strength information.
         * Provides flexibility to enable/disable RSSI on per-call basis.
         * 
         * @note enableRSSI parameter overrides device configuration
         * @warning Always call close() on returned container to free memory
         * 
         * @example Complete message reception:
         * @code
         * ResponseStructContainer response = e220ttl.receiveMessageComplete(20, true);
         * if (response.status.code == E220_SUCCESS) {
         *     Serial.print("RSSI: "); Serial.println(response.rssi);
         *     // Process received data...
         * }
         * response.close();
         * @endcode
         */
        ResponseStructContainer receiveMessageComplete(const uint8_t size, bool enableRSSI);
        
		/**
		 * @brief Receive complete string message with optional RSSI
		 * @param enableRSSI Whether to include RSSI information
		 * @return ResponseContainer with string data and optional RSSI
		 * 
		 * Receives a complete text message with optional signal strength information.
		 * Automatically handles string conversion and memory management.
		 * 
		 * @note Reads until available data is exhausted or buffer limit reached
		 * @note No explicit close() required for ResponseContainer
		 * 
		 * @example String message reception:
		 * @code
		 * ResponseContainer response = e220ttl.receiveMessageComplete(true);
		 * if (response.status.code == E220_SUCCESS) {
		 *     Serial.print("Message: "); Serial.println(response.data);
		 *     Serial.print("RSSI: "); Serial.println(response.rssi);
		 * }
		 * @endcode
		 */
		ResponseContainer receiveMessageComplete(bool enableRSSI);
	
		/**
		 * @brief Send string message
		 * @param message String to send
		 * @return ResponseStatus indicating success or failure
		 * 
		 * Sends a text string using the current transmission mode. The string
		 * is converted to bytes and transmitted with null terminator.
		 * 
		 * @note String length must not exceed 200 characters
		 * @note Device must be in normal transmission mode
		 * 
		 * @example Sending text message:
		 * @code
		 * ResponseStatus status = e220ttl.sendMessage("Hello, LoRa World!");
		 * if (status.code == E220_SUCCESS) {
		 *     Serial.println("Message sent successfully");
		 * }
		 * @endcode
		 */
		ResponseStatus sendMessage(const String message);
		
		/**
		 * @brief Receive available string message
		 * @return ResponseContainer with received string and status
		 * 
		 * Receives any available text message from the device buffer.
		 * Automatically converts received bytes to string format.
		 * 
		 * @note Returns immediately with available data or empty string
		 * @note No RSSI information included
		 * 
		 * @example Simple message reception:
		 * @code
		 * ResponseContainer response = e220ttl.receiveMessage();
		 * if (response.status.code == E220_SUCCESS && !response.data.isEmpty()) {
		 *     Serial.print("Received: "); Serial.println(response.data);
		 * }
		 * @endcode
		 */
		ResponseContainer receiveMessage();
		
		/**
		 * @brief Receive available string message with RSSI
		 * @return ResponseContainer with received string, RSSI, and status
		 * 
		 * Receives any available text message with signal strength information.
		 * Combines convenience of string handling with RSSI monitoring.
		 * 
		 * @note RSSI must be enabled in device configuration
		 * @note Returns immediately with available data
		 * 
		 * @example String reception with signal strength:
		 * @code
		 * ResponseContainer response = e220ttl.receiveMessageRSSI();
		 * if (response.status.code == E220_SUCCESS && !response.data.isEmpty()) {
		 *     Serial.print("Message: "); Serial.println(response.data);
		 *     Serial.print("Signal: "); Serial.print(response.rssi); Serial.println(" dBm");
		 * }
		 * @endcode
		 */
		ResponseContainer receiveMessageRSSI();
/** @} */ // End of Message Reception group

/**
 * @name Fixed Transmission
 * @brief Methods for addressed messaging using fixed transmission mode
 * @{
 */
		/**
		 * @brief Send fixed message to specific address (string)
		 * @param ADDH High address byte (0x00-0xFF)
		 * @param ADDL Low address byte (0x00-0xFF) 
		 * @param CHAN Channel number (0-255)
		 * @param message String message to send
		 * @return ResponseStatus indicating success or failure
		 * 
		 * Sends a text message to a specific device address and channel using
		 * fixed transmission mode. The address and channel parameters allow
		 * precise targeting of recipient devices.
		 * 
		 * @note Device will automatically switch to fixed transmission mode
		 * @note Target device must be configured with matching address/channel
		 * @note Message length must not exceed 197 bytes (200 - 3 address bytes)
		 * 
		 * @example Sending addressed message:
		 * @code
		 * ResponseStatus status = e220ttl.sendFixedMessage(0x00, 0x01, 23, "Hello Device 1!");
		 * if (status.code == E220_SUCCESS) {
		 *     Serial.println("Fixed message sent to address 0x0001, channel 23");
		 * }
		 * @endcode
		 */
		ResponseStatus sendFixedMessage(byte ADDH, byte ADDL, byte CHAN, const String message);

        /**
         * @brief Send fixed message to specific address (binary)
         * @param ADDH High address byte (0x00-0xFF)
         * @param ADDL Low address byte (0x00-0xFF)
         * @param CHAN Channel number (0-255)
         * @param message Pointer to binary data to send
         * @param size Number of bytes to send
         * @return ResponseStatus indicating success or failure
         * 
         * Sends binary data to a specific device address and channel using
         * fixed transmission mode. Allows sending structured or raw data
         * to precisely targeted devices.
         * 
         * @note Maximum data size is 197 bytes (200 - 3 address bytes)
         * @note Device automatically handles address prepending
         * 
         * @example Sending binary data to address:
         * @code
         * uint8_t sensorData[] = {0x01, 0x23, 0x45, 0x67};
         * ResponseStatus status = e220ttl.sendFixedMessage(0x00, 0x02, 15, 
         *                                                   sensorData, sizeof(sensorData));
         * if (status.code == E220_SUCCESS) {
         *     Serial.println("Sensor data sent to device 0x0002");
         * }
         * @endcode
         */
        ResponseStatus sendFixedMessage(byte ADDH,byte ADDL, byte CHAN, const void *message, const uint8_t size);
/** @} */ // End of Fixed Transmission group
/**
 * @name Broadcast Transmission
 * @brief Methods for sending messages to all devices on a channel
 * @{
 */
        /**
         * @brief Send broadcast message to all devices on channel (binary)
         * @param CHAN Channel number (0-255) 
         * @param message Pointer to binary data to broadcast
         * @param size Number of bytes to send
         * @return ResponseStatus indicating success or failure
         * 
         * Broadcasts binary data to all devices listening on the specified channel.
         * Uses the broadcast address (0xFFFF) to reach all devices regardless
         * of their individual addresses.
         * 
         * @note All devices on the channel will receive this message
         * @note Maximum data size is 197 bytes (200 - 3 address bytes)
         * @note Broadcast address is automatically set to 0xFFFF
         * 
         * @example Broadcasting sensor alert:
         * @code
         * uint8_t alertData[] = {0xFF, 0x01, 0x00}; // Alert code
         * ResponseStatus status = e220ttl.sendBroadcastFixedMessage(10, alertData, sizeof(alertData));
         * if (status.code == E220_SUCCESS) {
         *     Serial.println("Alert broadcast to all devices on channel 10");
         * }
         * @endcode
         */
        ResponseStatus sendBroadcastFixedMessage(byte CHAN, const void *message, const uint8_t size);
        
        /**
         * @brief Send broadcast message to all devices on channel (string)
         * @param CHAN Channel number (0-255)
         * @param message String message to broadcast
         * @return ResponseStatus indicating success or failure
         * 
         * Broadcasts a text message to all devices listening on the specified channel.
         * Convenient method for sending announcements or alerts to all devices.
         * 
         * @note All devices on the channel will receive this message
         * @note Message length must not exceed 197 characters
         * @note Uses broadcast address 0xFFFF automatically
         * 
         * @example Broadcasting announcement:
         * @code
         * ResponseStatus status = e220ttl.sendBroadcastFixedMessage(5, "System maintenance in 10 minutes");
         * if (status.code == E220_SUCCESS) {
         *     Serial.println("Maintenance announcement broadcast");
         * }
         * @endcode
         */
        ResponseStatus sendBroadcastFixedMessage(byte CHAN, const String message);
/** @} */ // End of Broadcast Transmission group

/**
 * @name Utility and Advanced Methods
 * @brief Additional methods for special operations and diagnostics
 * @{
 */
		/**
		 * @brief Receive initial message data
		 * @param size Number of bytes to receive initially
		 * @return ResponseContainer with received data and status
		 * 
		 * Receives the initial portion of an incoming message. Useful for
		 * reading message headers or determining full message length.
		 * 
		 * @note Primarily used for message parsing and protocol handling
		 * @note Remaining message data must be read with additional calls
		 * 
		 * @example Reading message header:
		 * @code
		 * ResponseContainer response = e220ttl.receiveInitialMessage(4);
		 * if (response.status.code == E220_SUCCESS) {
		 *     Serial.print("Header: "); Serial.println(response.data);
		 * }
		 * @endcode
		 */
		ResponseContainer receiveInitialMessage(const uint8_t size);

		/**
		 * @brief Send configuration message to remote device
		 * @param ADDH High address byte of target device
		 * @param ADDL Low address byte of target device
		 * @param CHAN Channel number of target device
		 * @param configuration Pointer to configuration structure to send
		 * @param programCommand Configuration save type (default: WRITE_CFG_PWR_DWN_SAVE)
		 * @return ResponseStatus indicating success or failure
		 * 
		 * Sends configuration parameters to a remote device over the air.
		 * This allows remote configuration of devices without physical access.
		 * 
		 * @note Target device must be in configuration mode to accept changes
		 * @note Both devices must be on same channel initially
		 * @warning Use with caution - incorrect parameters can make device unresponsive
		 * 
		 * @example Remote device configuration:
		 * @code
		 * Configuration newConfig;
		 * newConfig.CHAN = 15;
		 * newConfig.SPED.uartBaudRate = UART_BPS_19200;
		 * 
		 * ResponseStatus status = e220ttl.sendConfigurationMessage(0x00, 0x01, 23, 
		 *                                                          &newConfig, WRITE_CFG_PWR_DWN_SAVE);
		 * if (status.code == E220_SUCCESS) {
		 *     Serial.println("Remote configuration sent successfully");
		 * }
		 * @endcode
		 */
		ResponseStatus sendConfigurationMessage( byte ADDH,byte ADDL, byte CHAN, Configuration *configuration, PROGRAM_COMMAND programCommand = WRITE_CFG_PWR_DWN_SAVE);

        /**
         * @brief Check number of bytes available for reading
         * @return Number of bytes available in receive buffer
         * 
         * Returns the number of bytes currently available in the receive buffer.
         * Useful for non-blocking message reception and buffer management.
         * 
         * @note Returns immediately without waiting for data
         * @note Value may change between calls as data arrives
         * 
         * @example Checking for available data:
         * @code
         * int available = e220ttl.available();
         * if (available > 0) {
         *     Serial.print("Data available: "); Serial.print(available); Serial.println(" bytes");
         *     ResponseContainer response = e220ttl.receiveMessage();
         *     // Process received data...
         * }
         * @endcode
         */
        int available();
/** @} */ // End of Utility and Advanced Methods group
/**
 * @name Private Implementation Details
 * @brief Internal methods and data members for device management
 * 
 * These private members handle the low-level communication, mode management,
 * and hardware abstraction required for proper device operation.
 * 
 * @note These members are not intended for direct use by applications
 * @note Implementation details may change between library versions
 * @{
 */
	private:
		HardwareSerial* hs;  ///< Hardware Serial interface pointer

#ifdef ACTIVATE_SOFTWARE_SERIAL
		SoftwareSerial* ss;  ///< Software Serial interface pointer (when available)
#endif

		bool isSoftwareSerial = true;  ///< Flag indicating Software Serial usage

		int8_t txE220pin = -1;  ///< TX pin number (-1 if not used)
		int8_t rxE220pin = -1;  ///< RX pin number (-1 if not used)
		int8_t auxPin = -1;     ///< AUX status pin number (-1 if not used)

#ifdef HARDWARE_SERIAL_SELECTABLE_PIN
		uint32_t serialConfig = SERIAL_8N1;  ///< Serial configuration for ESP32
#endif

		int8_t m0Pin = -1;  ///< M0 mode control pin (-1 if not used)
		int8_t m1Pin = -1;  ///< M1 mode control pin (-1 if not used)

		unsigned long halfKeyloqKey = 0x06660708;  ///< Encryption key for security
		/**
		 * @brief Encrypt data using internal algorithm
		 * @param data Data to encrypt
		 * @return Encrypted data
		 */
		unsigned long encrypt(unsigned long data);
		/**
		 * @brief Decrypt data using internal algorithm  
		 * @param data Data to decrypt
		 * @return Decrypted data
		 */
		unsigned long decrypt(unsigned long data);

		UART_BPS_RATE bpsRate = UART_BPS_RATE_9600;  ///< Current UART baud rate

		struct NeedsStream {
			template<typename T>
			void begin(T &t, uint32_t baud) {
				DEBUG_PRINTLN("Begin ");
				t.setTimeout(500);
				t.begin(baud);
				stream = &t;
			}

#ifdef HARDWARE_SERIAL_SELECTABLE_PIN
//		  template< typename T >
//		  void begin( T &t, uint32_t baud, SerialConfig config ){
//			  DEBUG_PRINTLN("Begin ");
//			  t.setTimeout(500);
//			  t.begin(baud, config);
//			  stream = &t;
//		  }
//
			template< typename T >
			void begin( T &t, uint32_t baud, uint32_t config ) {
				DEBUG_PRINTLN("Begin ");
				t.setTimeout(500);
				t.begin(baud, config);
				stream = &t;
			}

			template< typename T >
			void begin( T &t, uint32_t baud, uint32_t config, int8_t txE220pin, int8_t rxE220pin ) {
				DEBUG_PRINTLN("Begin ");
				t.setTimeout(500);
				t.begin(baud, config, txE220pin, rxE220pin);
				stream = &t;
			}
#endif

			void listen() {}

			Stream *stream;
		};
		NeedsStream serialDef;

		MODE_TYPE mode = MODE_0_NORMAL;

		void managedDelay(unsigned long timeout);
		Status waitCompleteResponse(unsigned long timeout = 1000, unsigned int waitNoAux = 100);
		void flush();
		void cleanUARTBuffer();

		Status sendStruct(void *structureManaged, uint16_t size_);
		Status receiveStruct(void *structureManaged, uint16_t size_);
		bool writeProgramCommand(PROGRAM_COMMAND cmd, REGISTER_ADDRESS addr, PACKET_LENGHT pl);

		RESPONSE_STATUS checkUARTConfiguration(MODE_TYPE mode);

#ifdef LoRa_E220_DEBUG
		void printParameters(struct Configuration *configuration);
#endif
};

#endif

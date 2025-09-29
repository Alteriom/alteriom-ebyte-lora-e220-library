/**
 * @file statesNaming.h
 * @brief State definitions, constants, and utility functions for LoRa E220 Series
 * 
 * This file contains all the essential definitions for:
 * - Frequency band configuration
 * - Status codes and error definitions
 * - UART configuration parameters
 * - Air data rate settings
 * - Transmission power levels
 * - Device feature flags and options
 * - Utility functions for human-readable descriptions
 * 
 * @note This file is included by LoRa_E220.h and provides the foundation
 *       for all device configuration and status reporting
 * 
 * @author Renzo Mischianti (Original)
 * @author Alteriom (Documentation enhancements)
 * @version 1.1.5
 */

#include "Arduino.h"

//=============================================================================
// FREQUENCY BAND CONFIGURATION
//=============================================================================

/**
 * @brief Operating frequency base configuration
 * 
 * The E220 series operates in different frequency bands depending on the
 * device variant. The OPERATING_FREQUENCY defines the base frequency in MHz,
 * and individual channels are calculated as: base + channel_number.
 * 
 * Supported frequency bands:
 * - 433MHz band: FREQUENCY_433 (410-441MHz)
 * - 400MHz band: FREQUENCY_400 (410-441MHz)  
 * - 230MHz band: FREQUENCY_230 (220-251MHz)
 * - 868MHz band: FREQUENCY_868 (850-881MHz)
 * - 900MHz band: FREQUENCY_900 (850-881MHz)
 * - 915MHz band: FREQUENCY_915 (900-931MHz)
 * 
 * @note Default is 410MHz base frequency for 433MHz band devices
 * @note Channel 23 on 433MHz band = 410 + 23 = 433MHz
 * @warning Ensure your device variant matches the frequency band definition
 * 
 * @example Setting frequency band at compile time:
 * @code
 * #define FREQUENCY_433  // For 433MHz band devices
 * #include "LoRa_E220.h"
 * @endcode
 */
#ifdef FREQUENCY_433
	#define OPERATING_FREQUENCY 410  ///< Base frequency for 433MHz band (410-441MHz)
#elif defined(FREQUENCY_400)
	#define OPERATING_FREQUENCY 410  ///< Base frequency for 400MHz band (410-441MHz)
#elif defined(FREQUENCY_230)
	#define OPERATING_FREQUENCY 220  ///< Base frequency for 230MHz band (220-251MHz)
#elif defined(FREQUENCY_868)
	#define OPERATING_FREQUENCY 850  ///< Base frequency for 868MHz band (850-881MHz)
#elif defined(FREQUENCY_900)
	#define OPERATING_FREQUENCY 850  ///< Base frequency for 900MHz band (850-881MHz)
#elif defined(FREQUENCY_915)
	#define OPERATING_FREQUENCY 900  ///< Base frequency for 915MHz band (900-931MHz)
#else
	#define OPERATING_FREQUENCY 410  ///< Default base frequency (433MHz band)
#endif

/**
 * @brief Broadcast address for sending messages to all devices
 * 
 * When using fixed transmission mode, setting the address to BROADCAST_ADDRESS
 * will send the message to all devices listening on the specified channel,
 * regardless of their individual addresses.
 * 
 * @note Value 255 (0xFF) is reserved for broadcast messages
 * @note Both ADDH and ADDL should be set to 255 for full broadcast
 * 
 * @example Broadcasting to all devices:
 * @code
 * e220ttl.sendFixedMessage(BROADCAST_ADDRESS, BROADCAST_ADDRESS, 23, "Alert message");
 * @endcode
 */
#define BROADCAST_ADDRESS 255

//=============================================================================
// STATUS CODES AND ERROR DEFINITIONS
//=============================================================================

/**
 * @brief Response status codes for library operations
 * 
 * This enumeration defines all possible return codes from library operations.
 * Each code indicates a specific result or error condition, enabling precise
 * error handling and troubleshooting.
 * 
 * Success codes:
 * - E220_SUCCESS: Operation completed successfully
 * 
 * Error codes are prefixed with ERR_E220_ and indicate specific failure types:
 * - Hardware communication errors
 * - Configuration parameter errors  
 * - Protocol and timing errors
 * - Buffer and memory errors
 * 
 * @note Always check return status before using operation results
 * @note Use getResponseDescriptionByParams() for human-readable descriptions
 * 
 * @example Status checking pattern:
 * @code
 * ResponseStatus status = e220ttl.setConfiguration(config);
 * if (status.code != E220_SUCCESS) {
 *     Serial.println("Error: " + status.getResponseDescription());
 *     return;
 * }
 * // Proceed with successful operation...
 * @endcode
 */
typedef enum RESPONSE_STATUS {
#ifndef ARDUINO_ARCH_STM32
  SUCCESS = 1,                           ///< Generic success (STM32 compatibility)
#endif
  E220_SUCCESS = 1,                      ///< Operation completed successfully
  ERR_E220_UNKNOWN,                      ///< Unknown error - unexpected condition occurred
  ERR_E220_NOT_SUPPORT,                  ///< Feature not supported by device or platform
  ERR_E220_NOT_IMPLEMENT,                ///< Function not yet implemented
  ERR_E220_NOT_INITIAL,                  ///< Device not initialized - call begin() first
  ERR_E220_INVALID_PARAM,                ///< Invalid parameter value provided
  ERR_E220_DATA_SIZE_NOT_MATCH,          ///< Data size doesn't match expected length
  ERR_E220_BUF_TOO_SMALL,                ///< Buffer too small for operation
  ERR_E220_TIMEOUT,                      ///< Operation timed out waiting for response
  ERR_E220_HARDWARE,                     ///< Hardware communication failure
  ERR_E220_HEAD_NOT_RECOGNIZED,          ///< Response header not recognized
  ERR_E220_NO_RESPONSE_FROM_DEVICE,      ///< No response from device - check wiring
  ERR_E220_WRONG_UART_CONFIG,            ///< UART configuration error - use 9600bps for config
  ERR_E220_WRONG_FORMAT,                 ///< Invalid command or data format
  ERR_E220_PACKET_TOO_BIG                ///< Packet exceeds 200-byte limit
} Status;

/**
 * @brief Get human-readable description for status codes
 * @param status Status code to describe
 * @return String containing detailed description of the status
 * 
 * Converts status code enumerations into human-readable descriptions
 * for debugging and user feedback. Each status code has a specific
 * message that explains the condition and potential solutions.
 * 
 * @note Uses F() macro for flash string storage to save RAM
 * @note Returns "Invalid status!" for unknown status codes
 * 
 * @example Getting status description:
 * @code
 * Status result = someOperation();
 * if (result != E220_SUCCESS) {
 *     String description = getResponseDescriptionByParams(result);
 *     Serial.println("Operation failed: " + description);
 * }
 * @endcode
 * 
 * @see ResponseStatus::getResponseDescription() for member function version
 */
static String getResponseDescriptionByParams(byte status){
	switch (status)
	{
	  case E220_SUCCESS:
		return F("Success");                                            // Operation completed successfully
		break;
	  case ERR_E220_UNKNOWN:
		return F("Unknown");                                            // Unexpected error condition
		break;
	  case ERR_E220_NOT_SUPPORT:
		return F("Not support!");                                       // Feature not supported
		break;
	  case ERR_E220_NOT_IMPLEMENT:
		return F("Not implement");                                      // Function not implemented
		break;
	  case ERR_E220_NOT_INITIAL:
		return F("Not initial!");                                       // Device not initialized
		break;
	  case ERR_E220_INVALID_PARAM:
		return F("Invalid param!");                                     // Invalid parameter value
		break;
	  case ERR_E220_DATA_SIZE_NOT_MATCH:
		return F("Data size not match!");                               // Size mismatch error
		break;
	  case ERR_E220_BUF_TOO_SMALL:
		return F("Buff too small!");                                    // Buffer size insufficient
		break;
	  case ERR_E220_TIMEOUT:
		return F("Timeout!!");                                          // Operation timed out
		break;
	  case ERR_E220_HARDWARE:
		return F("Hardware error!");                                    // Hardware communication error
		break;
	  case ERR_E220_HEAD_NOT_RECOGNIZED:
		return F("Save mode returned not recognized!");                 // Invalid response header
		break;
	  case ERR_E220_NO_RESPONSE_FROM_DEVICE:
		return F("No response from device! (Check wiring)");            // Device not responding
		break;
	  case ERR_E220_WRONG_UART_CONFIG:
		return F("Wrong UART configuration! (BPS must be 9600 for configuration)"); // UART config error
		break;
	  case ERR_E220_PACKET_TOO_BIG:
		return F("The device support only 200byte of data transmission!"); // Packet size error
		break;
	  default:
		return F("Invalid status!");                                    // Unknown status code
	}
}

//=============================================================================
// UART CONFIGURATION PARAMETERS
//=============================================================================

/**
 * @brief UART parity configuration options
 * 
 * Defines the parity settings for UART communication with the E220 device.
 * These settings control error detection in serial communication:
 * 
 * - 8N1: 8 data bits, no parity, 1 stop bit (most common)
 * - 8O1: 8 data bits, odd parity, 1 stop bit
 * - 8E1: 8 data bits, even parity, 1 stop bit
 * 
 * @note Default setting is 8N1 (no parity checking)
 * @note Both devices must use the same parity setting
 * @warning Mismatched parity settings will cause communication errors
 * 
 * @example Setting UART parity:
 * @code
 * Configuration config;
 * config.SPED.uartParity = MODE_00_8N1;  // 8 data bits, no parity, 1 stop bit
 * @endcode
 */
enum E220_UART_PARITY
{
  MODE_00_8N1 = 0b00,  ///< 8 data bits, no parity, 1 stop bit (default)
  MODE_01_8O1 = 0b01,  ///< 8 data bits, odd parity, 1 stop bit
  MODE_10_8E1 = 0b10,  ///< 8 data bits, even parity, 1 stop bit  
  MODE_11_8N1 = 0b11   ///< 8 data bits, no parity, 1 stop bit (alternate)
};

/**
 * @brief Get human-readable UART parity description
 * @param uartParity Parity setting value (0-3)
 * @return String describing the parity configuration
 * 
 * Converts UART parity enumeration values into descriptive strings
 * for configuration display and debugging purposes.
 * 
 * @note Returns "Invalid UART Parity!" for unknown values
 * @example Displaying parity setting:
 * @code
 * String parity = getUARTParityDescriptionByParams(config.SPED.uartParity);
 * Serial.println("UART Parity: " + parity);
 * @endcode
 */
static String getUARTParityDescriptionByParams(byte uartParity){
	switch (uartParity)
	{
	  case MODE_00_8N1:
		return F("8N1 (Default)");      // 8 data bits, no parity, 1 stop bit
		break;
	  case MODE_01_8O1:
		return F("8O1");                // 8 data bits, odd parity, 1 stop bit
		break;
	  case MODE_10_8E1:
		return F("8E1");                // 8 data bits, even parity, 1 stop bit
		break;
	  case MODE_11_8N1:
		return F("8N1 (equal to 00");   // Alternative 8N1 mode
		break;
	  default:
		return F("Invalid UART Parity!");
	}
}

/**
 * @brief UART baud rate type enumeration (3-bit values)
 * 
 * Defines the available UART baud rates as 3-bit binary values
 * used in the device configuration registers. These correspond
 * to the actual baud rates defined in UART_BPS_RATE.
 * 
 * @note These are the binary values stored in configuration
 * @note Default is UART_BPS_9600 (0b011)
 * @warning Configuration mode requires 9600 bps regardless of this setting
 * 
 * @see UART_BPS_RATE for actual baud rate values
 */
enum UART_BPS_TYPE
{
  UART_BPS_1200 = 0b000,    ///< 1200 bps (3-bit config value)
  UART_BPS_2400 = 0b001,    ///< 2400 bps (3-bit config value)
  UART_BPS_4800 = 0b010,    ///< 4800 bps (3-bit config value)
  UART_BPS_9600 = 0b011,    ///< 9600 bps (3-bit config value, default)
  UART_BPS_19200 = 0b100,   ///< 19200 bps (3-bit config value)
  UART_BPS_38400 = 0b101,   ///< 38400 bps (3-bit config value)
  UART_BPS_57600 = 0b110,   ///< 57600 bps (3-bit config value)
  UART_BPS_115200 = 0b111   ///< 115200 bps (3-bit config value)
};

/**
 * @brief UART baud rate enumeration (actual values)
 * 
 * Defines the actual UART baud rate values in bits per second.
 * These are used in constructors and configuration to specify
 * the desired communication speed with the E220 device.
 * 
 * Supported rates:
 * - 1200 to 115200 bps
 * - Default: 9600 bps (reliable for most applications)
 * - Higher rates: Better for high-throughput applications
 * 
 * @note Configuration mode always uses 9600 bps regardless of this setting
 * @note Higher baud rates may be less reliable with Software Serial
 * @warning Ensure both device and controller support the selected rate
 * 
 * @example Setting baud rate in constructor:
 * @code
 * LoRa_E220 e220ttl(&Serial2, UART_BPS_RATE_19200);  // 19200 bps
 * @endcode
 */
enum UART_BPS_RATE
{
  UART_BPS_RATE_1200 = 1200,      ///< 1200 bits per second
  UART_BPS_RATE_2400 = 2400,      ///< 2400 bits per second
  UART_BPS_RATE_4800 = 4800,      ///< 4800 bits per second
  UART_BPS_RATE_9600 = 9600,      ///< 9600 bits per second (default)
  UART_BPS_RATE_19200 = 19200,    ///< 19200 bits per second
  UART_BPS_RATE_38400 = 38400,    ///< 38400 bits per second
  UART_BPS_RATE_57600 = 57600,    ///< 57600 bits per second
  UART_BPS_RATE_115200 = 115200   ///< 115200 bits per second
};

/**
 * @brief Get human-readable UART baud rate description
 * @param uartBaudRate Baud rate type value (0-7)
 * @return String describing the baud rate with "bps" suffix
 * 
 * Converts UART baud rate type enumeration into descriptive strings
 * showing the speed in bits per second. Useful for configuration
 * display and debugging.
 * 
 * @note Default rate (9600 bps) is marked with "(default)" suffix
 * @note Returns "Invalid UART Baud Rate!" for unknown values
 * 
 * @example Displaying baud rate:
 * @code
 * String rate = getUARTBaudRateDescriptionByParams(config.SPED.uartBaudRate);
 * Serial.println("UART Speed: " + rate);
 * @endcode
 */
static String getUARTBaudRateDescriptionByParams(byte uartBaudRate)
{
	switch (uartBaudRate)
	{
	  case UART_BPS_1200:
		return F("1200bps");              // 1200 bits per second
		break;
	  case UART_BPS_2400:
		return F("2400bps");              // 2400 bits per second
		break;
	  case UART_BPS_4800:
		return F("4800bps");              // 4800 bits per second
		break;
	  case UART_BPS_9600:
		return F("9600bps (default)");    // 9600 bits per second (standard)
		break;
	  case UART_BPS_19200:
		return F("19200bps");             // 19200 bits per second
		break;
	  case UART_BPS_38400:
		return F("38400bps");             // 38400 bits per second
		break;
	  case UART_BPS_57600:
		return F("57600bps");             // 57600 bits per second
		break;
	  case UART_BPS_115200:
		return F("115200bps");            // 115200 bits per second
		break;
	  default:
		return F("Invalid UART Baud Rate!");
	}
}

enum AIR_DATA_RATE
{
  AIR_DATA_RATE_000_24 = 0b000,
  AIR_DATA_RATE_001_24 = 0b001,
  AIR_DATA_RATE_010_24 = 0b010,
  AIR_DATA_RATE_011_48 = 0b011,
  AIR_DATA_RATE_100_96 = 0b100,
  AIR_DATA_RATE_101_192 = 0b101,
  AIR_DATA_RATE_110_384 = 0b110,
  AIR_DATA_RATE_111_625 = 0b111
};


static String getAirDataRateDescriptionByParams(byte airDataRate)
{
	switch (airDataRate)
	{
	  case AIR_DATA_RATE_000_24:
		return F("2.4kbps");
		break;
	  case AIR_DATA_RATE_001_24:
		return F("2.4kbps");
		break;
	  case AIR_DATA_RATE_010_24:
		return F("2.4kbps (default)");
		break;
	  case AIR_DATA_RATE_011_48:
		return F("4.8kbps");
		break;
	  case AIR_DATA_RATE_100_96:
		return F("9.6kbps");
		break;
	  case AIR_DATA_RATE_101_192:
		return F("19.2kbps");
		break;
	  case AIR_DATA_RATE_110_384:
		return F("38.4kbps");
		break;
	  case AIR_DATA_RATE_111_625:
		return F("62.5kbps");
		break;
	  default:
		return F("Invalid Air Data Rate!");
	}
}

enum SUB_PACKET_SETTING {
	SPS_200_00 = 0b00,
	SPS_128_01 = 0b01,
	SPS_064_10 = 0b10,
	SPS_032_11 = 0b11

};
static String getSubPacketSettingByParams(byte subPacketSetting)
{
	switch (subPacketSetting)
	{
	  case SPS_200_00:
		return F("200bytes (default)");
		break;
	  case SPS_128_01:
		  return F("128bytes");
		break;
	  case SPS_064_10:
		return F("64bytes");
		break;
	  case SPS_032_11:
		return F("32bytes");
		break;
	  default:
		return F("Invalid Sub Packet Setting!");
	}
}

enum RSSI_AMBIENT_NOISE_ENABLE {
	RSSI_AMBIENT_NOISE_ENABLED = 0b1,
	RSSI_AMBIENT_NOISE_DISABLED = 0b0
};
static String getRSSIAmbientNoiseEnableByParams(byte rssiAmbientNoiseEnabled)
{
	switch (rssiAmbientNoiseEnabled)
	{
	  case RSSI_AMBIENT_NOISE_ENABLED:
		return F("Enabled");
		break;
	  case RSSI_AMBIENT_NOISE_DISABLED:
		  return F("Disabled (default)");
		break;
	  default:
		return F("Invalid RSSI Ambient Noise enabled!");
	}
}

enum WOR_PERIOD {
	WOR_500_000 = 0b000,
	WOR_1000_001 = 0b001,
	WOR_1500_010 = 0b010,
	WOR_2000_011 = 0b011,
	WOR_2500_100 = 0b100,
	WOR_3000_101 = 0b101,
	WOR_3500_110 = 0b110,
	WOR_4000_111 = 0b111

};
static String getWORPeriodByParams(byte WORPeriod)
{
	switch (WORPeriod)
	{
	  case WOR_500_000:
		return F("500ms");
		break;
	  case WOR_1000_001:
			return F("1000ms");
		break;
	  case WOR_1500_010:
			return F("1500ms");
		break;
	  case WOR_2000_011:
			return F("2000ms (default)");
		break;
	  case WOR_2500_100:
			return F("2500ms");
		break;
	  case WOR_3000_101:
			return F("3000ms");
		break;
	  case WOR_3500_110:
			return F("3500ms");
		break;
	  case WOR_4000_111:
			return F("4000ms");
		break;
	  default:
		return F("Invalid WOR period!");
	}
}
enum LBT_ENABLE_BYTE {
	LBT_ENABLED = 0b1,
	LBT_DISABLED = 0b0
};
static String getLBTEnableByteByParams(byte LBTEnableByte)
{
	switch (LBTEnableByte)
	{
	  case LBT_ENABLED:
		return F("Enabled");
		break;
	  case LBT_DISABLED:
		  return F("Disabled (default)");
		break;
	  default:
		return F("Invalid LBT enable byte!");
	}
}

enum RSSI_ENABLE_BYTE {
	RSSI_ENABLED = 0b1,
	RSSI_DISABLED = 0b0
};
static String getRSSIEnableByteByParams(byte RSSIEnableByte)
{
	switch (RSSIEnableByte)
	{
	  case RSSI_ENABLED:
		return F("Enabled");
		break;
	  case RSSI_DISABLED:
		  return F("Disabled (default)");
		break;
	  default:
		return F("Invalid RSSI enable byte!");
	}
}

enum FIDEX_TRANSMISSION
{
  FT_TRANSPARENT_TRANSMISSION = 0b0,
  FT_FIXED_TRANSMISSION = 0b1
};


static String getFixedTransmissionDescriptionByParams(byte fixedTransmission)
{
	switch (fixedTransmission)
	{
	  case FT_TRANSPARENT_TRANSMISSION:
		return F("Transparent transmission (default)");
		break;
	  case FT_FIXED_TRANSMISSION:
		return F("Fixed transmission (first three bytes can be used as high/low address and channel)");
		break;
	  default:
		return F("Invalid fixed transmission param!");
	}
}

#ifdef E220_22
	enum TRANSMISSION_POWER
	{
	  POWER_22 = 0b00,
	  POWER_17 = 0b01,
	  POWER_13 = 0b10,
	  POWER_10 = 0b11

	};

	static String getTransmissionPowerDescriptionByParams(byte transmissionPower)
	{
		switch (transmissionPower)
		{
		  case POWER_22:
			return F("22dBm (Default)");
			break;
		  case POWER_17:
			return F("17dBm");
			break;
		  case POWER_13:
			return F("13dBm");
			break;
		  case POWER_10:
			return F("10dBm");
			break;
		  default:
			return F("Invalid transmission power param");
		}
	}
#elif defined(E220_30)
	enum TRANSMISSION_POWER
	{
	  POWER_30 = 0b00,
	  POWER_27 = 0b01,
	  POWER_24 = 0b10,
	  POWER_21 = 0b11

	};

	static String getTransmissionPowerDescriptionByParams(byte transmissionPower)
	{
		switch (transmissionPower)
		{
		  case POWER_30:
			return F("30dBm (Default)");
			break;
		  case POWER_27:
			return F("27dBm");
			break;
		  case POWER_24:
			return F("24dBm");
			break;
		  case POWER_21:
			return F("21dBm");
			break;
		  default:
			return F("Invalid transmission power param");
		}
	}
#else
	enum TRANSMISSION_POWER
	{
	  POWER_22 = 0b00,
	  POWER_17 = 0b01,
	  POWER_13 = 0b10,
	  POWER_10 = 0b11

	};

	static String getTransmissionPowerDescriptionByParams(byte transmissionPower)
	{
		switch (transmissionPower)
		{
		  case POWER_22:
			return F("22dBm (Default)");
			break;
		  case POWER_17:
			return F("17dBm");
			break;
		  case POWER_13:
			return F("13dBm");
			break;
		  case POWER_10:
			return F("10dBm");
			break;
		  default:
			return F("Invalid transmission power param");
		}
	}
#endif

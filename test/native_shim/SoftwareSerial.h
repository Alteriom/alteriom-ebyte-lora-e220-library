/*
 * SoftwareSerial for the native unit tests: the same in-memory UART as the
 * shim's HardwareSerial. See Arduino.h in this folder.
 */
#ifndef NATIVE_SHIM_SOFTWARESERIAL_H
#define NATIVE_SHIM_SOFTWARESERIAL_H

#include "Arduino.h"

class SoftwareSerial : public Stream {
public:
	SoftwareSerial(int rxPin, int txPin) : rxPin(rxPin), txPin(txPin) {}
	void begin(unsigned long baud) { this->baud = baud; }
	bool listen() { return true; }
	int rxPin;
	int txPin;
	unsigned long baud = 0;
};

#endif  // NATIVE_SHIM_SOFTWARESERIAL_H

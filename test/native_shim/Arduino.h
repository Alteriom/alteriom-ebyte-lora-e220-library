/*
 * Minimal Arduino core for the native unit tests (`pio test -e native`).
 *
 * It provides only what LoRa_E220.cpp and includes/statesNaming.h use, so
 * that the library's own source compiles and runs on the host:
 *   - String (backed by std::string)
 *   - Stream / HardwareSerial (an in-memory UART: bytes the library writes are
 *     captured, bytes queued with feed() are what it reads)
 *   - pinMode / digitalWrite / digitalRead (pin levels recorded per pin)
 *   - millis / delay on a simulated clock: every millis() call advances it by
 *     1 ms, so the library's busy-wait loops (managedDelay) terminate.
 *
 * Test-only: it is under test/, which neither the Arduino IDE nor the
 * PlatformIO library builder compiles for users.
 */
#ifndef NATIVE_SHIM_ARDUINO_H
#define NATIVE_SHIM_ARDUINO_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <deque>
#include <string>
#include <vector>

typedef uint8_t byte;
typedef bool boolean;

#define HIGH 0x1
#define LOW 0x0
#define INPUT 0x0
#define OUTPUT 0x1
#define DEC 10
#define HEX 16
#define BIN 2
#define SERIAL_8N1 0x06

#define F(string_literal) (string_literal)
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)

//-----------------------------------------------------------------------------
// Simulated clock and pins
//-----------------------------------------------------------------------------

namespace native_shim {
inline unsigned long &clock_ms() { static unsigned long ms = 0; return ms; }
inline int *pin_levels() { static int levels[256] = {0}; return levels; }
inline int *pin_modes() { static int modes[256] = {0}; return modes; }
inline void reset() {
	clock_ms() = 0;
	for (int i = 0; i < 256; i++) { pin_levels()[i] = LOW; pin_modes()[i] = INPUT; }
}
}  // namespace native_shim

inline unsigned long millis() { return ++native_shim::clock_ms(); }
inline void delay(unsigned long ms) { native_shim::clock_ms() += ms; }
inline void pinMode(uint8_t pin, uint8_t mode) { native_shim::pin_modes()[pin] = mode; }
inline void digitalWrite(uint8_t pin, uint8_t level) { native_shim::pin_levels()[pin] = level; }
inline int digitalRead(uint8_t pin) { return native_shim::pin_levels()[pin]; }

//-----------------------------------------------------------------------------
// String
//-----------------------------------------------------------------------------

class String {
public:
	String() {}
	String(const char *s) : value(s ? s : "") {}
	String(const std::string &s) : value(s) {}
	String(char c) : value(1, c) {}
	String(int n) : value(std::to_string(n)) {}
	String(unsigned int n) : value(std::to_string(n)) {}
	String(long n) : value(std::to_string(n)) {}
	String(unsigned long n) : value(std::to_string(n)) {}

	unsigned int length() const { return (unsigned int)value.size(); }
	const char *c_str() const { return value.c_str(); }
	char charAt(unsigned int i) const { return i < value.size() ? value[i] : 0; }
	String substring(unsigned int from, unsigned int to) const {
		if (from > to) { unsigned int t = from; from = to; to = t; }
		if (from >= value.size()) return String();
		if (to > value.size()) to = (unsigned int)value.size();
		return String(value.substr(from, to - from));
	}
	bool startsWith(const String &prefix) const { return value.compare(0, prefix.value.size(), prefix.value) == 0; }
	bool endsWith(const String &suffix) const {
		return suffix.value.size() <= value.size()
			&& value.compare(value.size() - suffix.value.size(), suffix.value.size(), suffix.value) == 0;
	}
	bool equals(const String &other) const { return value == other.value; }
	bool operator==(const String &other) const { return value == other.value; }
	bool operator==(const char *other) const { return value == (other ? other : ""); }
	String &operator+=(const String &other) { value += other.value; return *this; }
	friend String operator+(const String &a, const String &b) { return String(a.value + b.value); }
	friend String operator+(const String &a, const char *b) { return String(a.value + (b ? b : "")); }
	friend String operator+(const char *a, const String &b) { return String((a ? a : "") + b.value); }

private:
	std::string value;
};

//-----------------------------------------------------------------------------
// Stream and serial ports
//-----------------------------------------------------------------------------

class Stream {
public:
	virtual ~Stream() {}

	void setTimeout(unsigned long timeout) { this->timeout = timeout; }
	unsigned long getTimeout() const { return this->timeout; }

	int available() { return (int)rx.size(); }
	int peek() { return rx.empty() ? -1 : rx.front(); }
	int read() {
		if (rx.empty()) return -1;
		uint8_t b = rx.front();
		rx.pop_front();
		return b;
	}
	void flush() {}

	size_t write(uint8_t b) { onWrite(&b, 1); return 1; }
	size_t write(const uint8_t *buffer, size_t size) { onWrite(buffer, size); return size; }
	size_t write(const char *buffer, size_t size) { return write((const uint8_t *)buffer, size); }

	// Arduino's readBytes waits up to the timeout for each byte; the bytes a
	// test queued are all there already, so an empty queue is a timeout.
	size_t readBytes(char *buffer, size_t length) {
		size_t count = 0;
		while (count < length) {
			int c = read();
			if (c < 0) { delay(this->timeout); break; }
			buffer[count++] = (char)c;
		}
		return count;
	}
	size_t readBytes(uint8_t *buffer, size_t length) { return readBytes((char *)buffer, length); }

	String readString() {
		std::string s;
		int c;
		while ((c = read()) >= 0) s += (char)c;
		return String(s);
	}
	String readStringUntil(char terminator) {
		std::string s;
		int c;
		while ((c = read()) >= 0 && c != terminator) s += (char)c;
		return String(s);
	}

	// Test side: queue bytes for the library to read, and inspect what it wrote.
	void feed(const uint8_t *data, size_t size) { rx.insert(rx.end(), data, data + size); }
	std::vector<uint8_t> written;

protected:
	// Called for every write; a fake device overrides it to answer commands.
	virtual void onWrite(const uint8_t *data, size_t size) { written.insert(written.end(), data, data + size); }

	std::deque<uint8_t> rx;
	unsigned long timeout = 1000;
};

class HardwareSerial : public Stream {
public:
	void begin(unsigned long baud) { this->baud = baud; this->started = true; }
	void begin(unsigned long baud, uint32_t config) { begin(baud); this->config = config; }
	void end() { this->started = false; }
	unsigned long baud = 0;
	uint32_t config = SERIAL_8N1;
	bool started = false;
};

#endif  // NATIVE_SHIM_ARDUINO_H

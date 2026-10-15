/*
 * Compiles the library's own implementation into the native test binary, so
 * the unit tests exercise LoRa_E220.cpp and not a copy of it. The Arduino
 * types it needs come from test/native_shim/ (see platformio.ini, env:native).
 */
#include "../LoRa_E220.cpp"

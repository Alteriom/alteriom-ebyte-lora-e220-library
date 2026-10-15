---
verified_at: 2026-10-15
verified_commit: 6d202404f514ab13e80a302ead766aca96e29afd
---

# Architecture — where to look for each question

A flat Arduino library (sources at the root, `includes/` beside them), the layout `arduino-lint` checks (`.github/workflows/build-test.yml`). One class, `LoRa_E220`, talks to the module over a UART and three pins: M0/M1 select the mode, AUX says when the module is busy.

| Question | Owner |
|---|---|
| the public API: constructors, `begin`, `setMode`, `getConfiguration`/`setConfiguration`, `getModuleInformation`, the send and receive families, WOR | `LoRa_E220.h` (each method's Doxygen block has an example) |
| how a command is framed on the UART, waits on AUX, times out | `LoRa_E220.cpp` |
| the response wrappers and why `close()` must be called on a `ResponseStructContainer` | `LoRa_E220.h` (`ResponseStatus`, `ResponseStructContainer`, `ResponseContainer`) |
| register values and their descriptions: UART rate and parity, air data rate, sub-packet size, power, WOR period, RSSI, LBT, fixed transmission, the frequency band | `includes/statesNaming.h` |
| which serial a platform uses: `SoftwareSerial` except on STM32, ESP32, SAMD and mbed; on ESP32 a hardware serial also takes RX/TX pins and a serial config | `LoRa_E220.h` (`ACTIVATE_SOFTWARE_SERIAL` and `HARDWARE_SERIAL_SELECTABLE_PIN` near the top, and the constructor overloads) |
| the frequency band of a module (230/400/433/868/900/915 MHz): set by a `FREQUENCY_*` define before the include, 433/400 band by default | `includes/statesNaming.h` "FREQUENCY BAND CONFIGURATION" |
| debug output | `LoRa_E220.h` (`LoRa_E220_DEBUG`, `DEBUG_PRINT`) |
| the largest message: 200 bytes | `LoRa_E220.h` (`MAX_SIZE_TX_PACKET`) |
| one sketch per feature | `examples/` (`01_` configuration, `02_` transparent, `04_`/`05_` fixed, `06_` WOR, `07_` receive and broadcast); `keywords.txt` colours the API in the Arduino IDE |
| the upstream this forks | `docs/original-documentation.md` (the original author's documentation, kept as it was) |

## Things that are easy to break

- **Constructor overloads differ per platform**: 12 of them, the pin-only and `SoftwareSerial` ones only under `ACTIVATE_SOFTWARE_SERIAL`, the RX/TX-plus-config ones only on ESP32. A sketch that compiles on one board may pick a different overload, or none, on another; this is why CI compiles examples for AVR, ESP32 and ESP8266 (`build-test.yml`). `docs/PLATFORM_SPECIFIC_CONSTRUCTOR_SOLUTION.md` records the fix of #7.
- **Three include names point at one header**: `Alteriom_EByte_LoRa_E220.h` (advertised by `library.properties` `includes=`), `EByte_LoRa_E220_library.h` (named in `library.json` `headers`) and `LoRa_E220.h`. Existing sketches include each of them; removing one breaks them.
- **The version is written in four places** ([release.md](release.md)).

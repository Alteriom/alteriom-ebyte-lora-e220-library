# GitHub Copilot instructions — Alteriom EByte LoRa E220 library

Read `AGENTS.md` first. It is the entry for every agent on this repository: what the library is, where things live, and which job to take for what just happened. The facts live once, in `.github/icm/_shared/`: the library map (`architecture.md`), compiling and testing as CI does (`build-and-test.md`), releasing (`release.md`), Dependabot (`dependencies.md`), the documents (`documents.md`). When this file and those disagree, they are right.

## For Copilot specifically

- **Code style**: follow the existing code of `LoRa_E220.h` and `LoRa_E220.cpp`. Structs and enums have descriptive names (`Configuration`, `ModuleInformation`), register values have getter methods that return a readable description, debug output goes through `DEBUG_PRINT`/`DEBUG_PRINTLN`, and every `ResponseStructContainer` is released with `close()`.
- **Compatibility**: keep the public API and the three include names working; a constructor change must compile on every platform branch of `LoRa_E220.h` (`ACTIVATE_SOFTWARE_SERIAL`, `HARDWARE_SERIAL_SELECTABLE_PIN`).
- **Examples**: a new feature gets a sketch named `NN_featureName/NN_featureName.ino` under `examples/`, using the API as a user would.
- **Proof**: compile what you touched for AVR, ESP32 and ESP8266 as `build-test.yml` does. `pio test -e native` tests a mock, not the library; behaviour on the air needs a module.
- **Never** push a tag or publish a package: a person releases. A version bump is its own pull request, made in the release job (`AGENTS.md`, route `release`).

---
verified_at: 2026-10-15
verified_commit: b97a85eaa226bdf703382477db5769737fb0f14f
---

# Build, test, and what counts as evidence

What CI runs is `.github/workflows/build-test.yml`; read its steps rather than copying them anywhere.

## Run what CI runs

```bash
# compile a sketch against this checkout, as the matrix does (arduino-cli 1.0.4; install the cores first)
arduino-cli compile --fqbn arduino:avr:uno --library . examples/01_getConfiguration/01_getConfiguration.ino
arduino-cli compile --fqbn esp32:esp32:esp32 --library . examples/01_setConfiguration/01_setConfiguration.ino
arduino-cli compile --fqbn esp8266:esp8266:nodemcuv2 --library . examples/01_getConfiguration/01_getConfiguration.ino
pio test -e native                                      # the Unity tests in test/ (platformio.ini)
arduino-lint --library-manager update --compliance strict   # the layout and manifests, as the lint job does
```

The ESP32 and ESP8266 cores need their board-manager URLs (the "Install Arduino platforms" step). Use your own `arduino-cli` (CI installs 1.0.4 with `arduino/setup-arduino-cli`); `/bin/` is ignored for a local copy (`.gitignore`). `scripts/validate-library.sh` checks the manifests and the file layout before a release.

## What CI runs, and when

| Workflow, job | Runs | On |
|---|---|---|
| `build-test.yml` `arduino-compile-test` | the two `01_` sketches on `arduino:avr:uno`, `esp32:esp32:esp32`, `esp8266:esp8266:nodemcuv2` | push and pull request to `main`; called by `release.yml` |
| `build-test.yml` `platformio-test` | `pio test -e native` | same |
| `build-test.yml` `library-validation` | `arduino-lint`, strict, library-manager `update` | same |
| `test-platformio.yml` | `library.json` validation and compilation for Uno, ESP32, ESP8266 under PlatformIO | push to `main` touching `library.json` or `LoRa_E220.*`; dispatch |
| `documentation.yml` | checks the Docsify site and deploys `docs/` to GitHub Pages | push to `main` touching `docs/**` or any `.md`; pull requests build only |
| `update-wiki.yml`, `wiki-integration.yml` | push pages to the repository wiki; generate wiki templates | push to `main` touching `docs/`, `README.md`, `examples/` |
| `release.yml`, `platformio-publish.yml`, `arduino-submission.yml` | the release ([release.md](release.md)) | a `v*` tag; a published release; dispatch |
| `icm-check.yml` | the workspace files compile to the ICM standard | changes to the entry files or `.github/icm/`; weekly with `--strict` |

CodeQL runs as GitHub's default setup, weekly, not from a workflow here.

## Evidence

- **The native test compiles the library itself.** `test/library_source.cpp` includes `LoRa_E220.cpp` on top of the minimal Arduino core in `test/native_shim/` (`platformio.ini`, `env:native`), and `test/test_lora_e220.cpp` drives it against a fake module on the UART. A green `pio test -e native` proves the configuration and register-read logic, not radio behaviour or a platform `#if`.
- What does prove a change: every sketch it touches compiling for AVR, ESP32 and ESP8266 (more boards if it touches a platform `#if`), `arduino-lint` clean, and for behaviour on the air a run on a module with the serial output and the module variant (E220-400T22D, -900T30D, ...). Say which of these you have; if there is no module run, say so.

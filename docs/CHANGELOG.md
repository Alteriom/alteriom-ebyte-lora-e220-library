# Changelog

All notable changes to the Alteriom EByte LoRa E220 Series Library will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.1.3] - 2025-09-29

### Fixed
- Resolved NPM publishing workflow issue - v1.1.2 used old workflow version with `npm ci`
- Updated validation scripts to properly reference files in docs/ directory
- Ensured release workflow uses current version with fixed NPM publishing

### Note
This release ensures the corrected NPM publishing workflow is used, as GitHub Actions workflows are locked to the version when the tag was created.

## [1.1.2] - 2025-09-29

### Fixed
- Fixed NPM publishing workflow by changing `npm ci` to `npm install` (no package-lock.json existed)
- Updated library.properties with correct Alteriom maintainer information
- Added .npmignore file for cleaner NPM package distribution

### Added
- Created comprehensive Arduino Library Manager submission guide
- Added package-lock.json for proper dependency management
- Enhanced library.properties description to highlight Alteriom improvements

### Documentation
- Added ARDUINO_LIBRARY_SUBMISSION.md with step-by-step submission instructions
- Updated library metadata for Arduino Library Manager compliance

## [1.1.1] - 2025-09-29

### Security
- Updated GitHub Actions to latest secure versions:
  - arduino/setup-arduino-cli to v2.0.0 (from v1)
  - actions/setup-python to v5 (from v4) 
  - arduino/arduino-lint-action to v2 (from v1)
  - softprops/action-gh-release to v2 (from v1)
- Replaced deprecated eslint@8 with @eslint/js@9
- Removed non-existent MCP server dependencies from package.json
- All npm audit checks now pass with 0 vulnerabilities
- Added explicit permissions to GitHub Actions workflows

### Fixed
- Resolved security vulnerabilities in GitHub Actions dependencies
- Fixed workflow permissions for enhanced security

## [1.1.0] - 2025-09-29log

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.1.0] - 2025-09-29

### Added
- Enhanced MCP server integration and improved project structure
- Fixed GitHub Actions permissions for release workflow
- Optimized CI/CD pipeline to prevent duplicate workflow execution

### Fixed
- GitHub Actions 403 permission errors in release workflow
- Version consistency across all library files (LoRa_E220.h, library.properties, library.json, package.json)
- Repository URL corrections in library.json
- Duplicate workflow execution on tag pushes

### Changed
- Updated workflow structure for better efficiency and maintainability
- Made build-test.yml reusable to reduce code duplication
- Enhanced deployment guide with troubleshooting steps

## [1.0.0] - 2024-09-29

### Added
- Initial release of Alteriom fork of EByte LoRa E220 Series Library
- Enhanced CI/CD pipeline with GitHub Actions
- Automated release workflow for GitHub releases
- Arduino Library Manager compatibility
- PlatformIO support with multiple platform testing
- Comprehensive unit testing framework
- NPM package configuration for MCP server integration

### Changed
- Repository forked from original xreef/EByte_LoRa_E220_Series_Library
- Updated maintainer information to Alteriom
- Enhanced library metadata and descriptions
- Improved build and test automation

### Technical Details
- Full compatibility with Arduino, ESP32, ESP8266, STM32, and Raspberry Pi Pico
- LLCC68 chipset support
- 5-10km communication range
- Wake-on-Radio (WOR) power saving mode
- RSSI monitoring capabilities
- Multi-platform UART support (hardware and software serial)

## [0.x.x] - Previous Versions
- All previous versions maintained by original author Renzo Mischianti
- See original repository for historical changelog: https://github.com/xreef/EByte_LoRa_E220_Series_Library
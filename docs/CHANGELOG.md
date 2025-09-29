# Changelog

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
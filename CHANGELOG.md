# Changelog

All notable changes to the Alteriom EByte LoRa E220 Series Library will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Phase 3 documentation consolidation enhancements

## [1.1.6] - 2025-09-29

### Added
- **🚀 World-Class Doxygen Documentation System**
  - Complete Doxygen configuration with modern standards (Doxyfile)
  - Automated documentation generation workflow (.github/workflows/documentation.yml)
  - GitHub Pages deployment for live documentation site
  - 100% API coverage with comprehensive examples
  - Interactive examples with working Arduino sketches
  - Platform-specific documentation (Arduino, ESP32, STM32, Pi Pico)

- **🔧 Arduino Library Manager Automation**
  - Automated submission workflow for Arduino Library Registry
  - Smart detection of existing submissions in registry
  - Arduino indexer log monitoring and status tracking
  - Complete submission process documentation

### Fixed
- **📱 Arduino Library Manager Owner Display**
  - Updated library.json maintainer information to show Alteriom as current maintainer
  - Fixed ownership attribution in Arduino Library Manager
  - Enhanced author attribution while preserving original author credit

### Enhanced
- **📚 Documentation Infrastructure**
  - Enhanced README with comprehensive documentation section
  - Added live documentation badges and links
  - Created comprehensive documentation improvements guide
  - Future enhancement roadmap and recommendations
  - Automated quality validation and example checking

### Documentation
- Added docs/DOCUMENTATION_IMPROVEMENTS.md with complete implementation guide
- Updated README with live documentation links and features
- Enhanced package descriptions to highlight documentation improvements
- Added comprehensive automation summary (ARDUINO_AUTOMATION_SUMMARY.md)

### Technical
- VERSION: comment added to LoRa_E220.h for validation script compatibility
- All version numbers synchronized across library files
- Enhanced CI/CD with documentation generation and deployment

### LoRa Module Compatibility
- Maintained full compatibility with LLCC68 chipset
- Supports all E220 module variants (433MHz, 868MHz, 915MHz)
- Confirmed operation across all supported platforms

This release establishes industry-leading documentation standards and automation,
making the library accessible to developers from beginner to expert level.

## [1.1.5] - 2025-09-29

### Fixed
- Fixed wiki publishing error: corrected API-Reference.md file path
- Improved GitHub Packages error handling with better permissions
- Added continue-on-error for GitHub Packages to not fail entire release
- Enhanced error messages for GitHub Packages publishing issues

### Documentation
- Updated Arduino Library submission guide to reference v1.1.5

## [1.1.4] - 2025-09-29

### Added
- Dual package publishing: NPM Registry + GitHub Packages
- NPM package: `alteriom-ebyte-lora-e220` (public, no auth required)
- GitHub package: `@alteriom/alteriom-ebyte-lora-e220` (scoped, auth required)
- Updated documentation with both installation methods

### Enhanced
- Workflow now publishes to both registries simultaneously
- Added .npmrc configuration for GitHub Packages scope
- Enhanced package accessibility for different use cases

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
- LLCC68 chipset support for all E220 module variants
- 5-10km communication range capability
- Wake-on-Radio (WOR) power saving mode
- RSSI monitoring capabilities
- Multi-platform UART support (hardware and software serial)

### Hardware Compatibility
- **Chipset:** LLCC68 (Semtech LoRa transceiver)
- **Frequency Variants:** 433MHz, 868MHz, 915MHz
- **Platforms Tested:**
  - Arduino UNO/Nano/Mega (ATmega328P, ATmega2560)
  - ESP32 (all variants)
  - ESP8266 (NodeMCU, Wemos D1)
  - STM32 (Arduino framework)
  - Raspberry Pi Pico (RP2040)
  - SAMD21/SAMD51 boards

## [0.x.x] - Previous Versions

All previous versions (0.x.x) were maintained by original author Renzo Mischianti.

### Historical Context
- Original library created for EByte E220 LoRa modules
- Based on LLCC68 chipset communication protocols
- Established multi-platform Arduino support
- Developed comprehensive example set

For historical changelog of versions prior to 1.0.0, see the original repository:
https://github.com/xreef/EByte_LoRa_E220_Series_Library

---

## Version Numbering Scheme

This project follows [Semantic Versioning](https://semver.org/):
- **MAJOR** version: Incompatible API changes
- **MINOR** version: New functionality (backward compatible)
- **PATCH** version: Bug fixes (backward compatible)

## Release Notes

Detailed release notes for each version are available in:
- GitHub Releases: https://github.com/Alteriom/EByte_LoRa_E220_Series_Library/releases
- Documentation: https://alteriom.github.io/EByte_LoRa_E220_Series_Library/

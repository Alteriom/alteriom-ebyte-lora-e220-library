# 📦 PlatformIO Registry Publishing Guide

This guide covers how to publish the Alteriom EByte LoRa E220 Series Library to the PlatformIO Registry.

## 📋 **Current Status**

✅ **Library Configuration Ready**
- `library.json` - PlatformIO library manifest
- `library.properties` - Arduino library manifest  
- `platformio.ini` - Build configuration
- Valid repository structure

✅ **Automated Workflow Created**
- `.github/workflows/platformio-publish.yml` - Automated publishing
- Validation, testing, and publishing pipeline
- Multi-platform compilation testing

## 🚀 **Publishing Methods**

### **Method 1: Automated Publishing (Recommended)**

#### **Setup Authentication**

1. **Get PlatformIO Auth Token**:
   ```bash
   # Install PlatformIO Core
   pip install platformio
   
   # Login to your PlatformIO account
   pio account login
   
   # Get your authentication token
   pio account token
   ```

2. **Add GitHub Secret**:
   - Go to GitHub repository → Settings → Secrets and variables → Actions
   - Click "New repository secret"
   - Name: `PLATFORMIO_AUTH_TOKEN`
   - Value: Your PlatformIO auth token
   - Click "Add secret"

#### **Trigger Publishing**

**Option A: Release Tag (Automatic)**
```bash
# Create and push a new version tag
git tag v1.1.6
git push origin v1.1.6
```

**Option B: Manual Trigger**
- Go to GitHub → Actions → "PlatformIO Registry Publishing"
- Click "Run workflow"
- Select "force_publish: true" if needed

#### **Workflow Features**

The automated workflow includes:
- ✅ **Library validation** - Checks library.json syntax and required fields
- ✅ **Registry check** - Verifies if library/version already exists
- ✅ **Multi-platform testing** - Compiles for Arduino, ESP32, ESP8266
- ✅ **Automated publishing** - Publishes to PlatformIO Registry
- ✅ **Verification** - Confirms successful publication

### **Method 2: Manual Publishing**

#### **Prerequisites**
```bash
# Install PlatformIO Core
pip install platformio

# Authenticate
pio account login
```

#### **Validation**
```bash
# Validate library structure
pio package validate

# Check current registry status
pio lib search "Alteriom_EByte_LoRa_E220"
```

#### **Publish**
```bash
# From repository root directory
pio package publish

# Or specify path
pio package publish .
```

## 📝 **Library Configuration**

### **library.json Structure**
```json
{
  "name": "Alteriom_EByte_LoRa_E220",
  "version": "1.1.6",
  "keywords": "LoRa, UART, EByte, esp32, esp8266, stm32, SAMD, Arduino, Raspberry Pi Pico",
  "description": "Alteriom fork of Ebyte E220 LoRa library...",
  "homepage": "https://github.com/Alteriom/EByte_LoRa_E220_Series_Library",
  "authors": [...],
  "repository": {
    "type": "git",
    "url": "https://github.com/Alteriom/EByte_LoRa_E220_Series_Library"
  },
  "license": "MIT",
  "frameworks": "arduino",
  "platforms": "*",
  "headers": ["LoRa_E220.h", "EByte_LoRa_E220_library.h"]
}
```

### **Key Requirements**
- ✅ **Name**: Must be unique in registry
- ✅ **Version**: Semantic versioning (major.minor.patch)
- ✅ **Headers**: Array of main header files
- ✅ **Repository**: Public Git repository
- ✅ **License**: Valid SPDX license identifier

## 🧪 **Testing Before Publishing**

### **Local Testing**
```bash
# Create test project
mkdir test_platformio && cd test_platformio

# Initialize project
pio project init --board esp32

# Add library dependency to platformio.ini
echo "lib_deps = https://github.com/Alteriom/EByte_LoRa_E220_Series_Library.git" >> platformio.ini

# Test compilation
pio run
```

### **Multi-Platform Testing**
```bash
# Test different platforms
pio project init --board uno        # Arduino
pio project init --board esp32      # ESP32
pio project init --board nodemcuv2  # ESP8266
```

## 📦 **Installation Instructions (Post-Publishing)**

### **PlatformIO CLI**
```bash
# Install latest version
pio lib install "alteriom/Alteriom_EByte_LoRa_E220"

# Install specific version
pio lib install "alteriom/Alteriom_EByte_LoRa_E220@^1.1.6"

# Search for library
pio lib search "Alteriom_EByte_LoRa_E220"
```

### **platformio.ini Configuration**
```ini
[env:esp32]
platform = espressif32
board = esp32
framework = arduino
lib_deps = 
    alteriom/Alteriom_EByte_LoRa_E220@^1.1.6
```

### **Direct Git Installation**
```ini
lib_deps = 
    https://github.com/Alteriom/EByte_LoRa_E220_Series_Library.git
```

## 🔍 **Verification**

After successful publishing:

### **Registry Check**
- Visit: https://registry.platformio.org/libraries/alteriom/Alteriom_EByte_LoRa_E220
- Verify version and metadata

### **Installation Test**
```bash
# Create new project and test installation
mkdir verify_install && cd verify_install
pio project init --board esp32
echo "lib_deps = alteriom/Alteriom_EByte_LoRa_E220" >> platformio.ini

# Create test file
cat > src/main.cpp << 'EOF'
#include <Arduino.h>
#include "LoRa_E220.h"

LoRa_E220 e220ttl(2, 3, 4, 5, 6);

void setup() {
  Serial.begin(9600);
  e220ttl.begin();
}

void loop() {
  // Test code
}
EOF

# Compile
pio run
```

### **Search Verification**
```bash
pio lib search "Alteriom_EByte_LoRa_E220" --json-output
```

## 🔄 **Version Management**

### **Updating Versions**
1. **Update library.json**: Change version number
2. **Update library.properties**: Keep in sync with library.json
3. **Tag release**: Create Git tag matching version
4. **Publish**: Automated workflow will handle publishing

### **Version Strategy**
- **Patch** (x.x.1): Bug fixes, documentation updates
- **Minor** (x.1.x): New features, backward compatible
- **Major** (1.x.x): Breaking changes

## 🛠️ **Troubleshooting**

### **Common Issues**

#### **Authentication Errors**
```bash
# Re-authenticate
pio account logout
pio account login

# Verify authentication
pio account show
```

#### **Version Already Exists**
```
Error: Package version 1.1.6 already exists
```
**Solution**: Increment version in library.json

#### **Validation Errors**
```bash
# Check validation details
pio package validate --verbose

# Common fixes:
# - Fix library.json syntax
# - Add missing required fields
# - Ensure headers array is valid
```

#### **Build Failures**
```bash
# Test locally first
pio project init --board esp32
# Add lib_deps with local path
# Fix compilation issues before publishing
```

### **Library Structure Requirements**
```
EByte_LoRa_E220_Series_Library/
├── library.json          # PlatformIO manifest
├── library.properties    # Arduino manifest  
├── LoRa_E220.h           # Main header
├── LoRa_E220.cpp         # Implementation
├── EByte_LoRa_E220_library.h  # Convenience header
├── examples/             # Example sketches
├── includes/             # Additional headers
└── docs/                # Documentation
```

## 📈 **Post-Publishing**

### **Monitor Usage**
- Check download statistics in PlatformIO Registry
- Monitor GitHub issues and discussions
- Track community feedback

### **Documentation Updates**
- Update README.md with installation instructions
- Add PlatformIO badge to repository
- Update examples to reference published library

### **Cross-Platform Availability**
After PlatformIO publishing, the library will be available via:
- ✅ **PlatformIO Registry**: `pio lib install alteriom/Alteriom_EByte_LoRa_E220`
- ✅ **Arduino Library Manager**: Search "Alteriom_EByte_LoRa_E220"
- ✅ **NPM Package**: `npm install alteriom-ebyte-lora-e220`
- ✅ **GitHub**: Direct Git clone/download

## 🔗 **Resources**

- [PlatformIO Library Registry](https://registry.platformio.org/)
- [PlatformIO Package Specification](https://docs.platformio.org/en/latest/librarymanager/config.html)
- [Library Development Guide](https://docs.platformio.org/en/latest/librarymanager/ldf.html)
- [Semantic Versioning](https://semver.org/)

---

## 🎯 **Quick Start Summary**

For immediate publishing:

1. **Set up authentication token in GitHub Secrets**
2. **Update version in library.json if needed**
3. **Create and push a Git tag**: `git tag v1.1.6 && git push origin v1.1.6`
4. **Automated workflow will handle the rest!**

The library will be available for installation within minutes of successful publishing.
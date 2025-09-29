# Contributing to Alteriom EByte LoRa E220 Series Library

Thank you for your interest in contributing to the Alteriom EByte LoRa E220 Series Library! This document provides guidelines and information for contributors.

## 🤝 How to Contribute

### Reporting Issues
- Use the [GitHub Issues](https://github.com/Alteriom/EByte_LoRa_E220_Series_Library/issues) page
- Search existing issues before creating new ones
- Provide detailed information including:
  - Hardware setup (board, connections)
  - Software versions (Arduino IDE, PlatformIO, etc.)
  - Complete error messages
  - Minimal code example that reproduces the issue

### Suggesting Features
- Open an issue with the label "enhancement"
- Describe the feature and its use case
- Explain how it would benefit the library users

### Code Contributions
1. **Fork the repository**
2. **Create a feature branch**: `git checkout -b feature/your-feature-name`
3. **Make your changes** following our coding standards
4. **Test your changes** on multiple platforms when possible
5. **Update documentation** if needed
6. **Commit with clear messages**: `git commit -m "Add: clear description of changes"`
7. **Push to your fork**: `git push origin feature/your-feature-name`
8. **Create a Pull Request**

## 📋 Development Setup

### Prerequisites
- Arduino IDE 1.8.13+ or Arduino CLI
- PlatformIO (optional but recommended)
- Git

### Local Development
```bash
# Clone your fork
git clone https://github.com/YOUR-USERNAME/EByte_LoRa_E220_Series_Library.git
cd EByte_LoRa_E220_Series_Library

# Install dependencies (if using npm scripts)
npm install

# Build and test
pio run                    # Build for all platforms
pio test -e native        # Run unit tests
```

### Testing Your Changes
Before submitting a PR, please:

1. **Test compilation** on multiple platforms:
   - Arduino UNO (`pio run -e uno`)
   - ESP32 (`pio run -e esp32dev`)
   - ESP8266 (`pio run -e nodemcuv2`)

2. **Run unit tests**:
   ```bash
   pio test -e native
   ```

3. **Test examples**:
   - Verify at least 2-3 examples compile successfully
   - Test on actual hardware if possible

4. **Check library format**:
   ```bash
   arduino-cli lib lint .
   ```

## 🎯 Coding Standards

### Code Style
- Follow Arduino library conventions
- Use descriptive variable and function names
- Add comments for complex logic
- Keep functions focused and small

### File Organization
- Header files (`.h`) for declarations
- Source files (`.cpp`) for implementations
- Examples in `examples/` directory
- Tests in `test/` directory

### Naming Conventions
- **Classes**: PascalCase (`LoRa_E220`)
- **Functions**: camelCase (`getConfiguration`)
- **Variables**: camelCase (`responseStatus`)
- **Constants**: UPPER_SNAKE_CASE (`E220_SUCCESS`)
- **Files**: snake_case (`lora_e220.cpp`)

### Documentation
- Add JSDoc-style comments for public functions
- Update README.md if adding new features
- Include example code for new functionality
- Update CHANGELOG.md for notable changes

## 🔄 Pull Request Process

### Before Submitting
- [ ] Code compiles without warnings
- [ ] Tests pass on native platform
- [ ] Examples compile successfully
- [ ] Documentation updated
- [ ] CHANGELOG.md updated (if applicable)

### PR Description Template
```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Breaking change
- [ ] Documentation update

## Testing
- [ ] Tested on Arduino UNO
- [ ] Tested on ESP32
- [ ] Tested on ESP8266
- [ ] Unit tests pass
- [ ] Examples compile

## Additional Notes
Any additional information or context
```

### Review Process
1. Maintainers will review your PR
2. Feedback will be provided if changes are needed
3. Once approved, the PR will be merged
4. Changes will be included in the next release

## 🏷️ Versioning

We follow [Semantic Versioning](https://semver.org/):
- **MAJOR**: Breaking changes
- **MINOR**: New features (backward compatible)
- **PATCH**: Bug fixes (backward compatible)

### Version Update Process
Use the provided script to update versions:
```bash
./scripts/update-version.sh 1.0.1
```

This updates:
- `library.properties`
- `library.json`
- `package.json`
- `LoRa_E220.h`

## 🚀 Release Process

Releases are automated via GitHub Actions:

1. **Update version** using the script
2. **Update CHANGELOG.md** with new version details
3. **Commit changes**: `git commit -m "Bump version to v1.0.1"`
4. **Create tag**: `git tag -a v1.0.1 -m "Release v1.0.1"`
5. **Push**: `git push origin main --tags`

The GitHub Action will automatically:
- Create a GitHub release
- Generate release notes
- Provide Arduino Library Manager submission instructions

## 📝 Code of Conduct

Please note that this project is released with a [Contributor Code of Conduct](CODE_OF_CONDUCT.md). By participating in this project you agree to abide by its terms.

## ❓ Questions?

- **Technical questions**: Open an issue with the "question" label
- **General discussion**: Use GitHub Discussions
- **Security issues**: Email contact@alteriom.com

## 🙏 Recognition

Contributors will be recognized in:
- CHANGELOG.md for significant contributions
- GitHub contributors list
- Release notes when appropriate

Thank you for helping make this library better for everyone!
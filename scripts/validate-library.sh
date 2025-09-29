#!/bin/bash

# Library validation script for Alteriom EByte LoRa E220 Library
# Checks that all required files are present and properly configured

echo "🔍 Validating Alteriom EByte LoRa E220 Library..."
echo "================================================"

ERRORS=0
WARNINGS=0

# Check required files exist
echo "📁 Checking required files..."

REQUIRED_FILES=(
    "library.properties"
    "library.json"
    "LoRa_E220.h"
    "LoRa_E220.cpp"
    "Alteriom_EByte_LoRa_E220.h"
    "docs/CHANGELOG.md"
    "LICENSE.md"
    "examples"
    ".github/workflows/build-test.yml"
    ".github/workflows/release.yml"
    "README.md"
    "docs/DEPLOYMENT_GUIDE.md"
    "docs/CONTRIBUTING.md"
    "keywords.txt"
)

for file in "${REQUIRED_FILES[@]}"; do
    if [ -e "$file" ]; then
        echo "  ✅ $file"
    else
        echo "  ❌ $file (MISSING)"
        ((ERRORS++))
    fi
done

# Check version consistency
echo ""
echo "🔢 Checking version consistency..."

VERSION_PROPERTIES=$(grep "version=" library.properties | cut -d'=' -f2)
VERSION_JSON=$(grep '"version"' library.json | head -1 | cut -d'"' -f4)
VERSION_PACKAGE=$(grep '"version"' package.json | head -1 | cut -d'"' -f4)
VERSION_HEADER=$(grep "VERSION:" LoRa_E220.h | cut -d' ' -f4)

echo "  library.properties: $VERSION_PROPERTIES"
echo "  library.json:       $VERSION_JSON"
echo "  package.json:       $VERSION_PACKAGE"
echo "  LoRa_E220.h:        $VERSION_HEADER"

if [ "$VERSION_PROPERTIES" = "$VERSION_JSON" ] && [ "$VERSION_JSON" = "$VERSION_PACKAGE" ] && [ "$VERSION_PACKAGE" = "$VERSION_HEADER" ]; then
    echo "  ✅ All versions match: $VERSION_PROPERTIES"
else
    echo "  ❌ Version mismatch detected"
    ((ERRORS++))
fi

# Check repository URLs
echo ""
echo "🔗 Checking repository URLs..."

REPO_URL="https://github.com/Alteriom/EByte_LoRa_E220_Series_Library"

if grep -q "$REPO_URL" library.properties; then
    echo "  ✅ library.properties has correct URL"
else
    echo "  ❌ library.properties URL incorrect"
    ((ERRORS++))
fi

if grep -q "$REPO_URL" library.json; then
    echo "  ✅ library.json has correct URL"
else
    echo "  ❌ library.json URL incorrect"
    ((ERRORS++))
fi

# Check Arduino Library Manager compliance
echo ""
echo "📋 Checking Arduino Library Manager compliance..."

# Check library.properties format
if grep -q "name=Alteriom_EByte_LoRa_E220" library.properties; then
    echo "  ✅ Library name correct"
else
    echo "  ❌ Library name incorrect"
    ((ERRORS++))
fi

if grep -q "includes=Alteriom_EByte_LoRa_E220.h" library.properties; then
    echo "  ✅ Main header specified correctly"
else
    echo "  ❌ Main header not specified correctly"
    ((ERRORS++))
fi

# Check examples directory
if [ -d "examples" ]; then
    EXAMPLE_COUNT=$(find examples -name "*.ino" | wc -l)
    if [ $EXAMPLE_COUNT -gt 0 ]; then
        echo "  ✅ Examples directory contains $EXAMPLE_COUNT sketches"
    else
        echo "  ⚠️  Examples directory exists but no .ino files found"
        ((WARNINGS++))
    fi
else
    echo "  ❌ Examples directory missing"
    ((ERRORS++))
fi

# Check for basic header structure
echo ""
echo "🏗️  Checking header file structure..."

if grep -q "#ifndef LoRa_E220_h" LoRa_E220.h && grep -q "#define LoRa_E220_h" LoRa_E220.h; then
    echo "  ✅ Header guards present in LoRa_E220.h"
else
    echo "  ❌ Header guards missing in LoRa_E220.h"
    ((ERRORS++))
fi

if grep -q "Alteriom" LoRa_E220.h; then
    echo "  ✅ Alteriom attribution present"
else
    echo "  ⚠️  Alteriom attribution not found in header"
    ((WARNINGS++))
fi

# Check release readiness
echo ""
echo "🚀 Checking release readiness..."

# Check that CHANGELOG has current version entry
CURRENT_VERSION=$(grep "version=" library.properties | cut -d'=' -f2)
if grep -q "## \[$CURRENT_VERSION\]" docs/CHANGELOG.md; then
    echo "  ✅ CHANGELOG.md has entry for version $CURRENT_VERSION"
else
    echo "  ❌ CHANGELOG.md missing entry for version $CURRENT_VERSION"
    ((ERRORS++))
fi

# Check that GitHub workflows are valid YAML
if command -v python3 >/dev/null 2>&1; then
    python3 -c "import yaml; yaml.safe_load(open('.github/workflows/build-test.yml'))" 2>/dev/null
    if [ $? -eq 0 ]; then
        echo "  ✅ build-test.yml is valid YAML"
    else
        echo "  ❌ build-test.yml has YAML syntax errors"
        ((ERRORS++))
    fi
    
    python3 -c "import yaml; yaml.safe_load(open('.github/workflows/release.yml'))" 2>/dev/null
    if [ $? -eq 0 ]; then
        echo "  ✅ release.yml is valid YAML"
    else
        echo "  ❌ release.yml has YAML syntax errors"
        ((ERRORS++))
    fi
else
    echo "  ⚠️  Python3 not available, skipping YAML validation"
    ((WARNINGS++))
fi

# Check example count
EXAMPLE_COUNT=$(ls -1d examples/*/ 2>/dev/null | wc -l)
if [ $EXAMPLE_COUNT -ge 5 ]; then
    echo "  ✅ Examples directory contains $EXAMPLE_COUNT sketches (minimum 5)"
else
    echo "  ❌ Examples directory contains only $EXAMPLE_COUNT sketches (minimum 5 required)"
    ((ERRORS++))
fi

# Final results
echo ""
echo "📊 Validation Results"
echo "===================="

if [ $ERRORS -eq 0 ] && [ $WARNINGS -eq 0 ]; then
    echo "🎉 SUCCESS: Library validation passed with no issues!"
    exit 0
elif [ $ERRORS -eq 0 ]; then
    echo "⚠️  SUCCESS: Library validation passed with $WARNINGS warnings"
    exit 0
else
    echo "❌ FAILED: Library validation failed with $ERRORS errors and $WARNINGS warnings"
    echo ""
    echo "Please fix the errors above before proceeding with release."
    exit 1
fi
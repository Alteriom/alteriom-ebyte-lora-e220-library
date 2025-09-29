# PowerShell Library Validation Script for Alteriom EByte LoRa E220 Library

Write-Host "🔍 Validating Alteriom EByte LoRa E220 Library..." -ForegroundColor Cyan
Write-Host "================================================" -ForegroundColor Cyan

$errors = 0
$warnings = 0

# Required files check
Write-Host "`n📁 Checking required files..." -ForegroundColor Yellow

$requiredFiles = @(
    "library.properties",
    "library.json", 
    "LoRa_E220.h",
    "LoRa_E220.cpp",
    "Alteriom_EByte_LoRa_E220.h",
    "docs/CHANGELOG.md",
    "LICENSE.md",
    "examples",
    ".github/workflows/build-test.yml",
    ".github/workflows/release.yml",
    "README.md",
    "docs/DEPLOYMENT_GUIDE.md",
    "docs/CONTRIBUTING.md",
    "keywords.txt"
)

foreach ($file in $requiredFiles) {
    if (Test-Path $file) {
        Write-Host "  ✅ $file" -ForegroundColor Green
    } else {
        Write-Host "  ❌ $file (MISSING)" -ForegroundColor Red
        $errors++
    }
}

# Version consistency check  
Write-Host "`n🔢 Checking version consistency..." -ForegroundColor Yellow

$versionProperties = (Select-String -Path "library.properties" -Pattern "version=(.+)").Matches[0].Groups[1].Value
$versionJson = (Select-String -Path "library.json" -Pattern '"version":\s*"(.+)"').Matches[0].Groups[1].Value
$versionPackage = (Select-String -Path "package.json" -Pattern '"version":\s*"(.+)"').Matches[0].Groups[1].Value
$versionHeader = (Select-String -Path "LoRa_E220.h" -Pattern "VERSION:\s*(.+)").Matches[0].Groups[1].Value

Write-Host "  library.properties: $versionProperties"
Write-Host "  library.json:       $versionJson"
Write-Host "  package.json:       $versionPackage"
Write-Host "  LoRa_E220.h:        $versionHeader"

if ($versionProperties -eq $versionJson -and $versionJson -eq $versionPackage -and $versionPackage -eq $versionHeader) {
    Write-Host "  ✅ All versions match: $versionProperties" -ForegroundColor Green
} else {
    Write-Host "  ❌ Version mismatch detected" -ForegroundColor Red
    $errors++
}

# Repository URLs check
Write-Host "`n🔗 Checking repository URLs..." -ForegroundColor Yellow

$repoUrl = "https://github.com/Alteriom/EByte_LoRa_E220_Series_Library"

if (Select-String -Path "library.properties" -Pattern $repoUrl -Quiet) {
    Write-Host "  ✅ library.properties has correct URL" -ForegroundColor Green
} else {
    Write-Host "  ❌ library.properties URL incorrect" -ForegroundColor Red
    $errors++
}

if (Select-String -Path "library.json" -Pattern $repoUrl -Quiet) {
    Write-Host "  ✅ library.json has correct URL" -ForegroundColor Green
} else {
    Write-Host "  ❌ library.json URL incorrect" -ForegroundColor Red
    $errors++
}

# Arduino Library Manager compliance
Write-Host "`n📋 Checking Arduino Library Manager compliance..." -ForegroundColor Yellow

if (Select-String -Path "library.properties" -Pattern "name=Alteriom_EByte_LoRa_E220" -Quiet) {
    Write-Host "  ✅ Library name correct" -ForegroundColor Green
} else {
    Write-Host "  ❌ Library name incorrect" -ForegroundColor Red
    $errors++
}

if (Select-String -Path "library.properties" -Pattern "includes=Alteriom_EByte_LoRa_E220.h" -Quiet) {
    Write-Host "  ✅ Main header specified correctly" -ForegroundColor Green
} else {
    Write-Host "  ❌ Main header not specified correctly" -ForegroundColor Red
    $errors++
}

# Examples directory check
if (Test-Path "examples") {
    $exampleCount = (Get-ChildItem "examples" -Filter "*.ino" -Recurse).Count
    if ($exampleCount -gt 0) {
        Write-Host "  ✅ Examples directory contains $exampleCount sketches" -ForegroundColor Green
    } else {
        Write-Host "  ⚠️  Examples directory exists but no .ino files found" -ForegroundColor Yellow
        $warnings++
    }
} else {
    Write-Host "  ❌ Examples directory missing" -ForegroundColor Red
    $errors++
}

# Header file structure check
Write-Host "`n🏗️  Checking header file structure..." -ForegroundColor Yellow

if ((Select-String -Path "LoRa_E220.h" -Pattern "#ifndef LoRa_E220_h" -Quiet) -and 
    (Select-String -Path "LoRa_E220.h" -Pattern "#define LoRa_E220_h" -Quiet)) {
    Write-Host "  ✅ Header guards present in LoRa_E220.h" -ForegroundColor Green
} else {
    Write-Host "  ❌ Header guards missing in LoRa_E220.h" -ForegroundColor Red
    $errors++
}

if (Select-String -Path "LoRa_E220.h" -Pattern "Alteriom" -Quiet) {
    Write-Host "  ✅ Alteriom attribution present" -ForegroundColor Green
} else {
    Write-Host "  ⚠️  Alteriom attribution not found in header" -ForegroundColor Yellow
    $warnings++
}

# Release readiness check
Write-Host "`n🚀 Checking release readiness..." -ForegroundColor Yellow

$currentVersion = $versionProperties
if (Select-String -Path "docs/CHANGELOG.md" -Pattern "\[$currentVersion\]" -Quiet) {
    Write-Host "  ✅ CHANGELOG.md has entry for version $currentVersion" -ForegroundColor Green
} else {
    Write-Host "  ❌ CHANGELOG.md missing entry for version $currentVersion" -ForegroundColor Red
    $errors++
}

# Examples count check
$exampleDirs = (Get-ChildItem "examples" -Directory).Count
if ($exampleDirs -ge 5) {
    Write-Host "  ✅ Examples directory contains $exampleDirs example directories (minimum 5)" -ForegroundColor Green
} else {
    Write-Host "  ❌ Examples directory contains only $exampleDirs example directories (minimum 5 required)" -ForegroundColor Red
    $errors++
}

# Final results
Write-Host "`n📊 Validation Results" -ForegroundColor Cyan
Write-Host "====================" -ForegroundColor Cyan

if ($errors -eq 0 -and $warnings -eq 0) {
    Write-Host "🎉 SUCCESS: Library validation passed with no issues!" -ForegroundColor Green
    exit 0
} elseif ($errors -eq 0) {
    Write-Host "⚠️  SUCCESS: Library validation passed with $warnings warnings" -ForegroundColor Yellow
    exit 0
} else {
    Write-Host "❌ FAILED: Library validation failed with $errors errors and $warnings warnings" -ForegroundColor Red
    Write-Host "`nPlease fix the errors above before proceeding with release." -ForegroundColor Red
    exit 1
}
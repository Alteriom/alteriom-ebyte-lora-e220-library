#!/bin/bash

# Trigger Initial v1.0.0 Release Script
# This script creates and pushes the v1.0.0 tag to trigger automated release

set -e  # Exit on any error

echo "🚀 Triggering Initial v1.0.0 Release..."
echo "========================================"

# Ensure we're on main branch
echo "📍 Checking current branch..."
if [[ $(git branch --show-current) != "main" ]]; then
    echo "⚠️  Switching to main branch..."
    git checkout main
fi

# Pull latest changes
echo "🔄 Pulling latest changes from origin..."
git pull origin main

# Verify we're on the correct commit
CURRENT_COMMIT=$(git rev-parse --short HEAD)
EXPECTED_COMMIT="5920488"

echo "📝 Current commit: $CURRENT_COMMIT"
if [[ "$CURRENT_COMMIT" == "$EXPECTED_COMMIT" ]]; then
    echo "✅ On correct commit (merged PR #4)"
else
    echo "⚠️  Expected commit $EXPECTED_COMMIT, got $CURRENT_COMMIT"
    echo "   This might still be correct if there are newer commits"
fi

# Check if tag already exists
if git tag -l | grep -q "^v1.0.0$"; then
    echo "❌ Tag v1.0.0 already exists!"
    echo "   Use 'git tag -d v1.0.0' to delete it locally first if needed"
    exit 1
fi

# Run validation before tagging
echo "🔍 Running library validation..."
if ./scripts/validate-library.sh > /dev/null 2>&1; then
    echo "✅ Library validation passed"
else
    echo "❌ Library validation failed. Please fix issues before releasing."
    exit 1
fi

# Create the tag
echo "🏷️  Creating annotated tag v1.0.0..."
git tag -a v1.0.0 -m "Release v1.0.0 - Initial Alteriom fork with CI/CD"

# Push the tag
echo "📤 Pushing tag to origin..."
git push origin v1.0.0

echo ""
echo "🎉 SUCCESS: v1.0.0 tag created and pushed!"
echo ""
echo "📋 Next Steps:"
echo "1. Go to: https://github.com/Alteriom/EByte_LoRa_E220_Series_Library/actions"
echo "2. Watch the 'Create Release' workflow execute"
echo "3. Check the release at: https://github.com/Alteriom/EByte_LoRa_E220_Series_Library/releases"
echo "4. Follow the Arduino Library Manager submission instructions from the workflow output"
echo ""
echo "🚀 The automated release process is now running!"
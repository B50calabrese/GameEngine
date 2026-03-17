#!/bin/bash

# Descriptive Validation Script for ECS Subsystem
# This script outlines the manual and automated steps to verify ECS changes.

echo "### ECS Subsystem Validation Guide ###"

# 1. Automated Build Check
echo "Step 1: Verify Build"
echo "  Run: mkdir -p build && cd build && cmake .. && make GameEngine demos/ecsdemo"

# 2. Execution Check
echo "Step 2: Run ECS Demo"
echo "  Run: xvfb-run -a ./build/demos/ecsdemo/EcsDemo"
echo "  Expectation: The application should run without segfaults and demonstrate entity/component creation."

# 3. Unit Test Verification (if applicable)
echo "Step 3: Run ECS Unit Tests (Planned)"
echo "  Run: ./build/tests/ecs_tests (Wait for test infrastructure implementation)"

# 4. Code Style Verification
echo "Step 4: Verify Google C++ Style"
echo "  Run: clang-format -i include/engine/ecs/* src/engine/ecs/*"
echo "  Run: git diff --exit-code"

echo "#######################################"

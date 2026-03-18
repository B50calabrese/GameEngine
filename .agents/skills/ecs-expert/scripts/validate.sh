#!/bin/bash

# Descriptive Validation Script for ECS Subsystem
# This script distinguishes between core and integration verification.

echo "### ECS Subsystem Validation Guide ###"

# 🛠️ INTERNAL VERIFICATION (Engine Contributor)
echo "--- Core Subsystem Check (Internal) ---"
echo "1. Verify Registry and ComponentStorage Integrity"
echo "  Run: mkdir -p build && cd build && cmake .. && make GameEngine"
echo "  Goal: Ensure no breaking changes to ECS template logic or memory layouts."

# 🎮 INTEGRATION VERIFICATION (Game Developer)
echo "--- API Usage Check (Integration) ---"
echo "1. Run ECS Demo"
echo "  Run: xvfb-run -a ./build/demos/ecsdemo/EcsDemo"
echo "  Expectation: The application should run without segfaults and demonstrate entity creation/view iteration."

# 🧼 STYLE VERIFICATION
echo "--- Style Check ---"
echo "1. Verify Google C++ Style Compliance"
echo "  Run: clang-format -i include/engine/ecs/* src/engine/ecs/*"
echo "  Run: git diff --exit-code"

echo "#######################################"

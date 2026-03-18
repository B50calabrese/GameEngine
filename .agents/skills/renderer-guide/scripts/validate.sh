#!/bin/bash

# Descriptive Validation Script for Renderer Subsystem
# This script distinguishes between core rendering and batching verification.

echo "### Renderer Subsystem Validation Guide ###"

# 🛠️ INTERNAL VERIFICATION (Engine Contributor)
echo "--- Core Subsystem Check (Internal) ---"
echo "1. Verify Core Renderer and Shaders"
echo "  Run: mkdir -p build && cd build && cmake .. && make GameEngine demos/rendererdemo"
echo "  Run: xvfb-run -a ./build/demos/rendererdemo/RendererDemo"
echo "  Goal: Ensure the low-level rendering API (direct GL calls) is functioning correctly."

# 🎮 INTEGRATION VERIFICATION (Game Developer)
echo "--- API Usage Check (Integration) ---"
echo "1. Run RenderQueue and Batching Demo"
echo "  Run: make demos/renderqueuedemo"
echo "  Run: xvfb-run -a ./build/demos/renderqueuedemo/RenderQueueDemo"
echo "  Expectation: The application should render correctly with Z-sorting and texture-based batching."

# 🧼 STYLE VERIFICATION
echo "--- Style Check ---"
echo "1. Verify Google C++ Style Compliance"
echo "  Run: clang-format -i include/engine/graphics/* src/engine/graphics/*"
echo "  Run: git diff --exit-code"

echo "###########################################"

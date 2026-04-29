# Game Engine

A lightweight, modular 2D game engine written in C++17, featuring a custom Entity Component System (ECS), a batch-oriented 2D renderer, and support for Lua scripting.

## Features

- **Entity Component System (ECS):** A flexible and high-performance ECS for managing game objects and their behaviors.
- **2D Rendering:** Batch-based rendering system with support for sprites, primitive shapes, and custom shaders.
- **Input Management:** Easy-to-use action mapping system for keyboard and mouse input.
- **Lua Scripting:** Integrate game logic using Lua for rapid prototyping and flexibility.
- **UI System:** A built-in UI system for creating menus, HUDs, and other interface elements.
- **Physics:** Basic 2D physics system including collision detection and response.
- **Audio:** (TBD)
- **Developer Tools:** Integrated developer console and level editor.

## Getting Started

### Prerequisites

- C++17 compatible compiler (e.g., GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10 or higher
- System dependencies (Linux): `libgl1-mesa-dev`, `libglfw3-dev`, `libfreetype6-dev`, `pkg-config`

### Building from Source

```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

### Running Demos

The engine comes with various demos showcasing different subsystems. After building, you can find the executables in the `build/demos` directory.

```bash
./build/demos/breakout/breakout
```

## Directory Structure

- `include/engine`: Public header files for the engine.
- `src/engine`: Implementation files for the engine.
- `demos`: Collection of demo applications and games.
- `third_party`: External libraries (ImGui, Lua, JSON, Google Test).
- `tools`: Developer tools, including the level editor.

## Documentation

Documentation can be generated using Doxygen:

```bash
doxygen Doxyfile
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details (if available).

## AI Collaboration

This project is optimized for AI-driven development. Please refer to [AGENTS.md](AGENTS.md) for instructions on how to effectively contribute to this codebase as an AI agent.

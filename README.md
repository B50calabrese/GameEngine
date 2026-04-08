# Game Engine

A modular 2D game engine built with C++17, focusing on a clean Entity Component System (ECS) architecture, modern OpenGL rendering, and cross-platform compatibility.

## Project Structure

The project is organized into three main areas:

*   **Engine (`src/engine`, `include/engine`)**: The core library providing ECS, rendering, input handling, scene management, and utility systems.
*   **Tools (`src/tools`)**: Development tools built on top of the engine, such as the Level Editor.
*   **Demos (`demos/`)**: A wide variety of example applications and full games showcasing engine features:
    *   **Games**: Full examples like `breakout`, `platformer`, `rpg`, and `tactical-rpg`.
    *   **Feature Demos**: Focused examples for lighting, physics, scripting (Lua), UI, animations, and more.

## Dependencies

All major dependencies are included as git submodules and are built from source automatically by the CMake build system:

*   **GLFW (v3.4)**: Window management and input handling.
*   **FreeType (v2.13.3)**: Font loading and glyph rendering.
*   **ImGui (v1.91.0)**: Immediate mode graphical user interface.
*   **nlohmann/json (v3.11.3)**: Modern JSON for C++.
*   **Lua (v5.4)**: Embedded scripting language.
*   **sol2 (v3.3.0)**: C++ <-> Lua bindings.
*   **GLM**: OpenGL Mathematics library (header-only).
*   **GLAD**: OpenGL loader (included in `third_party/glad`).
*   **STB**: Single-file public domain libraries for images.
*   **Google Test (v1.15.2)**: Unit testing framework.

## Building the Project

### Prerequisites

*   A C++17 compliant compiler (MSVC, GCC, or Clang).
*   CMake 3.10 or higher.
*   **Linux specific dependencies**:
    ```bash
    sudo apt-get install libgl1-mesa-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
    ```

### Build Instructions

1.  **Clone the repository with submodules**:
    ```bash
    git clone --recursive <repository-url>
    ```
    If you've already cloned without submodules, run:
    ```bash
    git submodule update --init --recursive
    ```

2.  **Generate and build**:
    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build .
    ```

### Running Demos

After a successful build, the demo executables can be found in the `build/demos/` directory tree. For example:
*   `build/demos/games/breakout/breakout`
*   `build/demos/lightingdemo/lightingdemo`

## License

[Add License Information Here]

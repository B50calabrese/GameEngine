# Project: 2D Game Engine (AI Collaboration Layer)

Welcome to the 2D Game Engine project. This repository is optimized for AI collaboration using a machine-readable context layer. As a Senior Systems Architect, I have established the following project map and guidelines.

## Project Map & Subsystem Directory

| Subsystem | Directory | Description | Skill Link |
| :--- | :--- | :--- | :--- |
| **Core** | `src/engine/core`, `include/engine/core` | Application lifecycle, Windowing, JobSystem | (TBD) |
| **ECS** | `src/engine/ecs`, `include/engine/ecs` | Entity Component System (Registry, View, Storage) | [ecs-expert](.agents/skills/ecs-expert/SKILL.md) |
| **Renderer** | `src/engine/graphics`, `include/engine/graphics` | OpenGL context, RenderQueue, Post-processing | [renderer-guide](.agents/skills/renderer-guide/SKILL.md) |
| **Input** | `src/engine/input`, `include/engine/input` | Physical input and Action mapping | [input-mapper](.agents/skills/input-mapper/SKILL.md) |
| **Asset Mgmt** | `src/engine/util`, `include/engine/util` | Resource caching and thread-safe loading | [asset-manager](.agents/skills/asset-manager/SKILL.md) |
| **UI** | `src/engine/ui`, `include/engine/ui` | Hybrid Retained ECS UI system | [ui-architect](.agents/skills/ui-architect/SKILL.md) |

## Build Instructions (C++17)

This project requires a C++17 compliant compiler and the following dependencies:
- **Dependencies**: `glfw3`, `gl1-mesa`, `freetype2`, `xvfb` (for headless testing).
- **Build System**: `CMake` (minimum 3.10).

### Standard Build:
```bash
mkdir -p build && cd build
cmake ..
make -j$(nproc)
```

### Running Demos (Headless):
```bash
xvfb-run -a ./build/demos/windowdemo/WindowDemo
```

## Mandates for AI Collaborators

- **Google C++ Style Guide**: All C++ code and documentation must strictly follow the Google C++ Style Guide. Use `clang-format` with the provided `.clang-format` file.
- **Self-Evolution**: Whenever you learn a new pattern, resolve a bug, or discover a 'Gotcha' while running builds, you are **mandated** to update the relevant `SKILL.md` or this `AGENTS.md` immediately.
- **Verification**: Always run the subsystem's validation script (e.g., `.agents/skills/ecs-expert/scripts/validate.sh`) before submitting changes.
- **Incremental Changes**: Focus on small, traceable commits. Link documentation rules to 'Golden Sample' code files in the `demos/` directory using XML tags (e.g., `<golden_sample file="..." />`).

## Directory Hierarchy Tags (m.css)

To ensure the documentation tree index is correctly generated in the `m.css` Doxygen theme, every directory must be documented with `@dir` tags in its primary header or implementation file.

- **Root include**: `@dir include/engine` (in `include/engine/core/engine.h`)
- **Root src**: `@dir src/engine` (in `src/engine/core/engine.cpp`)

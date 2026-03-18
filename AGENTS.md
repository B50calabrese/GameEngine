# Project: 2D Game Engine (AI Collaboration Layer)

This repository is optimized for AI-driven development. As a Senior Systems Architect, I have established this context layer to guide agents based on their specific operational scope.

## Operational Contexts

To maximize effectiveness, first identify your **Persona Context**:

### A. Engine Contributor (Core Development)
*   **Goal**: Modify, optimize, or extend the engine's internal systems (`include/engine`, `src/engine`).
*   **Mandate**: Maintain strict ABI/API stability. Follow internal memory management patterns (e.g., `ComponentStorage` reallocation rules).
*   **Primary Skills**: `ecs-expert`, `renderer-guide`.

### B. Game Developer (Application Logic)
*   **Goal**: Build games or demos using the engine's public API (`demos/`, `include/engine`).
*   **Mandate**: Use the high-level `Registry` and `RenderQueue` APIs. Do **not** touch internal buffers or low-level GL calls.
*   **Primary Skills**: `input-mapper`, `ui-architect`, `asset-manager`.

---

## Project Map & Subsystem Directory

| Subsystem | Directory | Scope | Skill Link |
| :--- | :--- | :--- | :--- |
| **Core** | `src/engine/core` | Internal | (TBD) |
| **ECS** | `src/engine/ecs` | Core/App | [ecs-expert](.agents/skills/ecs-expert/SKILL.md) |
| **Renderer** | `src/engine/graphics` | Core/App | [renderer-guide](.agents/skills/renderer-guide/SKILL.md) |
| **Input** | `src/engine/input` | Application | [input-mapper](.agents/skills/input-mapper/SKILL.md) |
| **Asset Mgmt** | `src/engine/util` | Application | [asset-manager](.agents/skills/asset-manager/SKILL.md) |
| **UI** | `src/engine/ui` | Application | [ui-architect](.agents/skills/ui-architect/SKILL.md) |

---

## Ruthless Mandates for AI Collaborators

1.  **Strict Google C++ Style**: No exceptions. Braces for all control structures. Pointers for output parameters.
2.  **Snapshot Iteration**: All ECS systems must assume the state is a snapshot. Never delete while iterating; use a `deferred_destruction` list.
3.  **Queue-First Rendering**: Never call `Renderer` methods directly from game logic. All draw calls **must** pass through `RenderQueue`.
4.  **Self-Evolution**: If you find a bug or a new 'Gotcha' (e.g., a specific OpenGL driver quirk or a threading race condition), you are **mandated** to update the relevant `SKILL.md` before submitting your fix.
5.  **Validation**: Run the relevant `scripts/validate.sh` before submission. Failure to validate is a failure of the persona.

## Build Instructions (C++17)

- **Standard Build**: `mkdir -p build && cd build && cmake .. && make -j$(nproc)`
- **Headless Demos**: `xvfb-run -a ./build/demos/<demo_name>/<ExecutableName>`
- **Formatting**: `find include src demos -name "*.h" -o -name "*.cpp" | xargs clang-format -i`

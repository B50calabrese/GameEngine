# Project: 2D Game Engine (AI Collaboration Layer)

This repository is optimized for AI-driven development. It follows a clean, modular architecture with strict adherence to Google C++ Style and single responsibility principles.

## Operational Contexts

### A. Engine Contributor (Core Development)
*   **Goal**: Modify, optimize, or extend internal systems.
*   **Mandate**: Maintain ABI/API stability. Follow internal patterns.
*   **Key Systems**:
    *   **Core**: Lifecycle management, windowing, job system (`engine::Application`, `engine::Engine`, `engine::Window`, `engine::core::JobSystem`).
    *   **ECS**: Entity-Component-System framework (`engine::ecs::Registry`, `engine::ecs::EntityManager`).
    *   **Graphics**: Low-level rendering, shaders, textures, batching (`engine::graphics::Renderer`, `engine::graphics::PrimitiveRenderer`).

### B. Game Developer (Application Logic)
*   **Goal**: Build games or demos using the public API.
*   **Mandate**: Use `SceneManager`, `Registry`, and `RenderQueue`. Avoid low-level GL calls.
*   **Templates**: Use the games in `demos/games/` as templates for structure and best practices.

---

## Subsystem Documentation

| Subsystem | Namespace | Description |
| :--- | :--- | :--- |
| **Core** | `engine` | Main application loop, windowing, and global engine state. |
| **ECS** | `engine::ecs` | Entity management, component storage, and event dispatching. |
| **Graphics** | `engine::graphics` | Rendering pipeline, including batching and post-processing. |
| **Input** | `engine` | Keyboard and mouse input handling via `InputManager`. |
| **Scene** | `engine` | Scene stack management via `SceneManager`. |
| **UI** | `engine::ui` | Declarative UI system integrated with ECS. |
| **Util** | `engine::util` | Scripting, logging, and common utilities. |

---

## Technical Mandates

1.  **Google C++ Style**: Mandatory. PascalCase for types/functions, snake_case for locals/parameters, trailing underscore for private members.
2.  **Interface Design**: One header per C++ file. Keep interfaces minimal and cohesive.
3.  **No `using namespace`**: Forbidden in headers. Discouraged in source files.
4.  **Queue-First Rendering**: All draw calls must pass through `RenderQueue::Default().Submit()`.
5.  **Single Responsibility**: Each class/system should do one thing well.

---

## Testing

Basic unit tests are located in `src/tests/`.
- To run tests: `g++ -Iinclude -Ithird_party/include -Ithird_party/glm src/tests/test_ecs.cpp src/engine/ecs/entity_manager.cpp -o test_ecs && ./test_ecs`
- Future agents should add tests for new core functionality.

---

## Build & Format

- **Build**: `mkdir build && cd build && cmake .. && make`
- **Format**: `find include src demos -name "*.h" -o -name "*.cpp" | xargs clang-format -i`

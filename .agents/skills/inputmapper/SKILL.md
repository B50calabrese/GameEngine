---
id: input-mapper
subsystem: input
version: 0.2.0
author_persona: Senior Systems Architect
---

# Input Mapping Skill (STUB)

<guiding_principles>
- **Action Decoupling**: Systems must query named actions (e.g., "Jump") rather than raw physical keys (e.g., `KEY_SPACE`) via `ActionManager`.
- **Zero Allocation**: The `ActionManager` and `InputManager` must not allocate memory during the main frame update.
</guiding_principles>

## Contextual Instructions

### 🛠️ Engine Contributor Context (Core Development)
- **Goal**: Add new device support or input events to `InputManager`.
- **Constraint**: Maintain the event-based callback system for external window interactions.

### 🎮 Game Developer Context (Application Logic)
- **Goal**: Map and query user input.
- **Constraint**: Always check `InputManager::IsConsumed()` to avoid input conflicts with UI.
- **Golden Sample**: <golden_sample file="demos/actionmapping/main.cpp" />

## Subsystem Architecture

- `InputManager`: A singleton that handles raw device events (keyboard/mouse).
- `ActionManager`: Maps logical actions to physical inputs.

## Gotchas

- **Input Consumption**: UI elements can consume input events, preventing them from reaching game systems. Use `InputManager::IsConsumed()` or `ActionManager::IsConsumed()`.
- **State Update Timing**: `InputManager::UpdateState()` must be called **exactly once** per frame, and it should be called **before** `glfwPollEvents()` to correctly preserve the previous frame's state for `IsKeyPressed` and `IsKeyReleased` checks. In this engine, this is handled automatically within `Window::PollEvents()`. Calling it again elsewhere (like in the main loop) will break single-frame event detection.
- **KeyCode Naming**: The `KeyCode` enumerators follow the `kPascalCase` naming convention (e.g., `kSpace`, `kEscape`). Using `KC_` prefixes or all-caps is deprecated.

## Validation

(TBD) Refer to `demos/actionmapping/main.cpp`.

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

## Validation

(TBD) Refer to `demos/actionmapping/main.cpp`.

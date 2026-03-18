---
id: ui-architect
subsystem: ui
version: 0.2.0
author_persona: Senior Systems Architect
---

# UI Architecture Skill (STUB)

<guiding_principles>
- **Hybrid Retained ECS**: The UI system uses specialized ECS components (`UITransform`, `UIInteractable`) managed by UI-specific systems (`UILayoutSystem`, `UIRenderSystem`).
- **Resolution Independence**: UI elements should use anchors and relative positioning for screen-space stability across different window resolutions.
- **Reactive Data Binding**: Use `UIBinding` components to synchronize ECS component state with UI text or progress bars.
</guiding_principles>

## Contextual Instructions

### 🛠️ Engine Contributor Context (Core Development)
- **Goal**: Maintain the `UILayoutSystem` or `UIRenderSystem`.
- **Constraint**: Ensure UI is rendered in a separate batch from the game world. Use `Renderer::Flush()` to isolate screen-space commands.
- **Gotcha**: UI systems must process input *before* game systems to correctly set the `is_consumed` flag.

### 🎮 Game Developer Context (Application Logic)
- **Goal**: Create UI layouts and interactions.
- **Constraint**: Always check `IsConsumed()` before handling game-world input.
- **Golden Sample**: <golden_sample file="demos/ui_menu/main.cpp" />

## Subsystem Architecture

- `UITransform`: Manages hierarchical screen-space layout.
- `UIInteractable`: Handles click and hover states.
- `UIInputSystem`: Prevents 'click-through' to the game world.

## Gotchas

- **Click-Through**: Systems must check `IsConsumed()` before processing input to avoid simultaneous interaction with the UI and world.
- **Z-Ordering**: UI elements must be rendered with high Z-order values to stay on top of the world-space batch.

## Validation

(TBD) Refer to `demos/ui_menu/main.cpp`.

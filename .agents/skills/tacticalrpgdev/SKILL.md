---
id: tacticalrpg-dev
name: Tactical RPG Developer
description: Skills for extending the Tactical RPG Roguelike demo.
---

# Tactical RPG Developer

This skill provides guidance on extending and maintaining the Tactical RPG Roguelike demo, which follows a fully data-driven ECS architecture.

<guiding_principles>
- **ECS-First**: Maintain the separation of data (components) and logic (systems). Avoid raw pointers to internal class state.
- **Modularity**: Keep components, systems, and class registrations in their individual files within their respective subdirectories.
- **Data-Driven**: Use `ActionRegistry` and `ClassRegistry` to define game content. New spells/abilities should be defined as data entities with effect components.
- **Shared Entities**: Standard actions (like "Firebolt") are shared entities in the Scene Registry. Use `ActionRegistry::GetOrCreateAction` to fetch them.
</guiding_principles>

## Development Skills

### How to add a new Class
1. Add a new entry to the `ClassType` enum in `demos/games/tacticalrpg/game_types.h`.
2. Update `ClassTypeToString` in `demos/games/tacticalrpg/game_types.h`.
3. Create a new registration file: `demos/games/tacticalrpg/classes/<your_class>.cpp`.
4. Implement `void Register<YourClass>(ClassRegistry& registry)` and add it as a `friend` in `class_registry.h`.
5. Call the new registration function in `ClassRegistry::RegisterAll()`.
6. Add the new file to `TACTICAL_RPG_SOURCES` in `CMakeLists.txt`.

### How to add a new Action (Spell/Ability)
1. Define the action data in `demos/games/tacticalrpg/action_registry.cpp` within `RegisterAll()`.
2. Specify the effects using component data (dice size, count, modifier).
3. If a new effect type is needed, add a corresponding component in `action_components.h` and update `CombatSystem::ApplyEffect` to handle it.

### Architecture Overview
- **Entities**: Characters, Map Tiles, and Actions are all first-class Entities in the ECS.
- **Components**:
  - `IdentityComponent`: Name and Class.
  - `Stats`: Core attributes and HP.
  - `TurnStateComponent`: Initiative and action availability.
  - `GridPositionComponent`: Logical coordinate on the NxN grid.
  - `ActionDataComponent` / `EffectsListComponent`: Attached to Action entities.
- **Systems**:
  - `SyncTransformSystem`: Bridges logical grid positions to the engine's `TransformComponent`.
  - `CombatSystem`: Resolves the interaction between an Action entity and a Target entity.

## Gotchas
- **Registry Lifetime**: `ActionRegistry` caches entity IDs. Call `ActionRegistry::Get().ClearCache()` when the Scene's Registry is destroyed or swapped (e.g., in `OnAttach`).
- **Transform Sync**: Always use `GridPositionComponent` for logical movement; the `SyncTransformSystem` will automatically update the visual position for the renderer.
- **Friendly Classes**: Registration functions for individual classes must be declared as `friend` in `ClassRegistry` to access the private `RegisterClass` method.

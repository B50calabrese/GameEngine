---
id: tactical-rpg-dev
name: Tactical RPG Developer
description: Skills for extending the Tactical RPG Roguelike demo.
---

# Tactical RPG Developer

This skill provides guidance on extending and maintaining the Tactical RPG Roguelike demo.

<guiding_principles>
- Maintain the modular architecture by keeping logic separated into specialized classes (e.g., `TurnManager`, `CombatRules`).
- Use the `ActionRegistry` and `ClassRegistry` for adding new content rather than hardcoding.
- Prefer data-driven definitions over complex switch statements.
- Ensure all new actions use the `Effect` system for consistent behavior.
</guiding_principles>

## Development Skills

### How to add a new Class
1. Add a new entry to the `ClassType` enum in `demos/games/tactical-rpg/game_types.h`.
2. Update `ClassTypeToString` in `demos/games/tactical-rpg/game_types.h` to include the new class name.
3. In `demos/games/tactical-rpg/class_registry.cpp`, add a new block in `RegisterAll()` to define the base stats and initial actions for the new class.

### How to add a new Action (Spell/Ability)
1. If it's a completely new mechanic, you might need to add a new `ActionType` in `demos/games/tactical-rpg/game_types.h`.
2. Define the action in `demos/games/tactical-rpg/action_registry.cpp` by adding it to `RegisterAll()`.
3. Specify the effects (e.g., `DamageEffect`, `HealEffect`) that the action should apply.
4. If the action has unique logic not covered by existing effects, implement a new `Effect` subclass in `demos/games/tactical-rpg/effect.h/cpp`.

### How to add a new Map Node Type
1. Add a new entry to the `NodeType` enum in `demos/games/tactical-rpg/map_node.h`.
2. Create a new subclass of `MapNode` in `demos/games/tactical-rpg/map_node.h` and implement `OnEnter()` and `OnExit()`.
3. Update `MapSystem::GenerateMap` in `demos/games/tactical-rpg/map_system.h` to instantiate your new node type.
4. Update the rendering logic in `MapScene::OnRender` in `demos/games/tactical-rpg/map_scene.h` to include a label or icon for the new type.

## Gotchas
- The `TurnManager` processes recurring effects at the start of a turn; ensure any temporary buffs/debuffs are correctly added to the character's `status_effects` list.
- Movement cost and special terrain effects should be validated via `CombatRules`.
- Coordinate systems for the map and grid use different scales; ensure visual positions are correctly mapped from logical grid/node coordinates.

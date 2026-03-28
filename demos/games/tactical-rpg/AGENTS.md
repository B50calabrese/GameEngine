# Tactical RPG Roguelike Demo - Agent Guidance

This demo implements a top-down tactical RPG with grid-based combat and a directed graph map (Slay the Spire style).

## Architecture

- **GameManager**: Singleton that manages the high-level game state (party, map, floor).
- **ClassRegistry**: Central location for defining the 12 D&D classes, their stats, and starting actions.
- **BattleScene**: Coordinates the battle flow using the modular systems below. Now manages entity IDs for the party and enemies.
- **BattleGrid**: Manages the NxN grid using ECS. Each tile is an individual Entity with a `TileComponent`. A single Entity with `GridMapComponent` provides spatial lookups.
- **GridRenderer**: Renders the `BattleGrid` by iterating over `GridMapComponent` and `TileComponent`.
- **TurnManager**: Handles initiative rolls and manages the current turn state using `TurnStateComponent`. Operates on the ECS registry.
- **EnemyAI**: Implements basic tactical decision-making. Now operates on Entity IDs and fetches data via components (`GridPositionComponent`, `Stats`, etc.).
- **CombatRules**: Centralized logic for validating movement and range, operating on Entity IDs.
- **Systems**: Decoupled gameplay logic moved to the `systems/` directory:
  - `SyncTransformSystem`: Synchronizes `GridPositionComponent` to the engine's core `TransformComponent`.
  - `CombatSystem`: Resolves data-driven combat effects (damage/heal) attached to Action entities.
- **MapScene**: Handles the navigation through the generated directed graph.
- **GameTypes**: Contains POD structs for `Character`, `Stats`, `Action`, etc.

## How to Extend

### Adding New Enemies
Currently, enemies are generated using the same `ClassRegistry` but with an `is_enemy` flag. You can create specialized enemy classes by adding new logic to `BattleScene::SetupEnemies`.

### Terrain Effects
`BattleScene::HandlePlayerTurn` already checks for `TerrainType::Damage` (Lava). You can add more complex effects (like Slow reducing movement points) in the movement logic.

### Assets
The game currently uses primitive squares for characters and tiles.
- Use `engine::ecs::components::Sprite` and `engine::graphics::ecs::SpriteRenderSystem` to integrate textures.
- You can find assets in `demos/assets/` or add new ones there.

## Current Limitations (Minimal Commit)
- **Architecture**: Refactored to ECS! Data is stored in components, and logic is handled by systems or Registry-aware managers.
- **AI**: Very basic (moves toward closest player and attacks).
- **Targeting**: Only cardinal for movement/cursor (as requested).
- **Persistence**: Single session only.
- **Balance**: Initial stats and damage values are estimated; need playtesting.

## Development Commands
Build the demo:
```bash
mkdir -p build && cd build
cmake ..
make TacticalRpg
```
Run the demo (requires X11/Headless setup):
```bash
./demos/TacticalRpg/TacticalRpg
```

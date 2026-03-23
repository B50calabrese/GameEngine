# Tactical RPG Roguelike Demo - Agent Guidance

This demo implements a top-down tactical RPG with grid-based combat and a directed graph map (Slay the Spire style).

## Architecture

- **GameManager**: Singleton that manages the high-level game state (party, map, floor).
- **ClassRegistry**: Central location for defining the 12 D&D classes, their stats, and starting actions.
- **BattleScene**: Coordinates the battle flow using the modular systems below.
- **BattleGrid**: Manages the NxN grid, terrain, and rendering of the battlefield.
- **TurnManager**: Handles initiative rolls and manages the current turn state for all characters.
- **EnemyAI**: Implements basic tactical decision-making for enemy characters.
- **CombatRules**: Centralized logic for validating movement, range, and other game rules.
- **MapScene**: Handles the navigation through the generated directed graph.
- **GameTypes**: Contains POD structs for `Character`, `Stats`, `Action`, etc.

## Development Skills

### How to add a new Class
1. Add a new entry to the `ClassType` enum in `game_types.h`.
2. Update `ClassTypeToString` in `game_types.h` to include the new class.
3. In `class_registry.cpp`, add a new block in `RegisterAll()` to define the base stats and initial actions for the new class.

### How to add a new Action (Spell/Ability)
1. If it's a completely new mechanic, you might need to add a new `ActionType` in `game_types.h`.
2. Define the action in `ClassRegistry::RegisterAll()` by adding it to a class's `base_actions`.
3. If the action has unique logic (like healing or a specific buff), update `BattleScene::HandlePlayerTurn` in `battle_scene.cpp` to handle the new action by its name or type.

## How to Extend

### Adding New Enemies
Currently, enemies are generated using the same `ClassRegistry` but with an `is_enemy` flag. You can create specialized enemy classes by adding new logic to `BattleScene::SetupEnemies`.

### Terrain Effects
`BattleScene::HandlePlayerTurn` already checks for `TerrainType::Damage` (Lava). You can add more complex effects (like Slow reducing movement points) in the movement logic.

### Assets
The game currently uses primitive squares for characters and tiles.
- Use `engine::graphics::SpriteComponent` and `engine::graphics::SpriteRenderSystem` to integrate textures.
- You can find assets in `demos/assets/` or add new ones there.

## Current Limitations (Minimal Commit)
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

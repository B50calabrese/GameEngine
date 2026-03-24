# Tactical RPG Roguelike Demo - Agent Guidance

This demo implements a top-down tactical RPG with grid-based combat and a directed graph map (Slay the Spire style).

## Architecture

- **GameManager**: Singleton that manages the high-level game state (party, map, floor).
- **ClassRegistry**: Central location for defining the 12 D&D classes, their stats, and starting actions.
- **BattleScene**: Coordinates the battle flow using the modular systems below.
- **BattleGrid**: Manages the NxN grid, terrain, and state of the battlefield.
- **GridRenderer**: Specialized utility for rendering the `BattleGrid`.
- **TurnManager**: Handles initiative rolls and manages the current turn state for all characters.
- **EnemyAI**: Implements basic tactical decision-making for enemy characters.
- **CombatRules**: Centralized logic for validating movement, range, and other game rules.
- **MapScene**: Handles the navigation through the generated directed graph.
- **GameTypes**: Contains POD structs for `Character`, `Stats`, `Action`, etc.

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
- **Architecture**: Terrain types are currently part of the `BattleGrid` logic rather than being represented as ECS components.
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

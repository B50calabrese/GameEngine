#ifndef DEMOS_GAMES_TACTICAL_RPG_BATTLE_GRID_H_
#define DEMOS_GAMES_TACTICAL_RPG_BATTLE_GRID_H_

#include <vector>

#include <engine/ecs/registry.h>
#include <glm/glm.hpp>

#include "game_types.h"

namespace tactical_rpg {

struct GridConfig {
  float slow_chance = 0.1f;
  float damage_chance = 0.05f;
  float impassible_chance = 0.05f;
};

class BattleGrid {
 public:
  static const int kSize = 10;

  static void Setup(engine::ecs::Registry& registry,
                    const GridConfig& config = GridConfig());

  static TerrainType GetTerrain(engine::ecs::Registry& registry, int x, int y);
  static bool IsWalkable(engine::ecs::Registry& registry, int x, int y);
  static bool IsInBounds(int x, int y);
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_BATTLE_GRID_H_

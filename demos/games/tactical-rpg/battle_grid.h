#ifndef DEMOS_GAMES_TACTICAL_RPG_BATTLE_GRID_H_
#define DEMOS_GAMES_TACTICAL_RPG_BATTLE_GRID_H_

#include <vector>

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

  BattleGrid();
  void Setup(const GridConfig& config = GridConfig());
  TerrainType GetTerrain(int x, int y) const;
  bool IsWalkable(int x, int y) const;
  bool IsInBounds(int x, int y) const;

 private:
  TerrainType grid_[kSize][kSize];
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_BATTLE_GRID_H_

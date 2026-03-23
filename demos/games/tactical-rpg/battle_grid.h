#ifndef DEMOS_GAMES_TACTICAL_RPG_BATTLE_GRID_H_
#define DEMOS_GAMES_TACTICAL_RPG_BATTLE_GRID_H_

#include <vector>

#include <glm/glm.hpp>

#include "game_types.h"

namespace tactical_rpg {

class BattleGrid {
 public:
  static const int kSize = 10;

  BattleGrid();
  void Setup();
  TerrainType GetTerrain(int x, int y) const;
  bool IsWalkable(int x, int y) const;
  bool IsInBounds(int x, int y) const;

  void Render(const glm::vec2& offset, float tile_size,
              const glm::ivec2& cursor_pos);

 private:
  TerrainType grid_[kSize][kSize];
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_BATTLE_GRID_H_

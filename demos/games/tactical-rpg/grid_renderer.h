#ifndef DEMOS_GAMES_TACTICAL_RPG_GRID_RENDERER_H_
#define DEMOS_GAMES_TACTICAL_RPG_GRID_RENDERER_H_

#include <glm/glm.hpp>

#include "battle_grid.h"

namespace tactical_rpg {

class GridRenderer {
 public:
  static void Render(const BattleGrid& grid, const glm::vec2& offset,
                     float tile_size, const glm::ivec2& cursor_pos);
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_GRID_RENDERER_H_

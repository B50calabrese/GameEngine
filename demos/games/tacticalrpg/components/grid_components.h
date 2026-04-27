#ifndef DEMOS_GAMES_TACTICAL_RPG_COMPONENTS_GRID_COMPONENTS_H_
#define DEMOS_GAMES_TACTICAL_RPG_COMPONENTS_GRID_COMPONENTS_H_

#include <glm/glm.hpp>

#include <engine/ecs/entity_manager.h>

#include "../game_types.h"

namespace tactical_rpg {

struct TileComponent {
  TerrainType terrain;
  glm::ivec2 grid_pos;
};

struct GridMapComponent {
  static constexpr int kSize = 10;
  engine::ecs::EntityID tiles[kSize][kSize];
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_COMPONENTS_GRID_COMPONENTS_H_

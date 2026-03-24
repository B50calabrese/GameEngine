#ifndef DEMOS_GAMES_TACTICAL_RPG_SYSTEMS_SYNC_TRANSFORM_SYSTEM_H_
#define DEMOS_GAMES_TACTICAL_RPG_SYSTEMS_SYNC_TRANSFORM_SYSTEM_H_

#include <engine/ecs/registry.h>
#include <glm/glm.hpp>

namespace tactical_rpg {

class SyncTransformSystem {
 public:
  static void Update(engine::ecs::Registry& registry, float tile_size,
                     const glm::vec2& grid_offset);
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_SYSTEMS_SYNC_TRANSFORM_SYSTEM_H_

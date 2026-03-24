#ifndef DEMOS_GAMES_TACTICAL_RPG_SYSTEMS_H_
#define DEMOS_GAMES_TACTICAL_RPG_SYSTEMS_H_

#include <engine/ecs/registry.h>
#include <glm/glm.hpp>

namespace tactical_rpg {

class SyncTransformSystem {
 public:
  static void Update(engine::ecs::Registry& registry, float tile_size,
                     const glm::vec2& grid_offset);
};

class CombatSystem {
 public:
  static void ApplyEffect(engine::ecs::Registry& registry,
                          engine::ecs::EntityID action_entity,
                          engine::ecs::EntityID target_entity);
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_SYSTEMS_H_

#ifndef DEMOS_GAMES_TACTICAL_RPG_SYSTEMS_COMBAT_SYSTEM_H_
#define DEMOS_GAMES_TACTICAL_RPG_SYSTEMS_COMBAT_SYSTEM_H_

#include <engine/ecs/registry.h>

namespace tactical_rpg {

class CombatSystem {
 public:
  static int ApplyEffect(engine::ecs::Registry& registry,
                         engine::ecs::EntityID action_entity,
                         engine::ecs::EntityID target_entity);
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_SYSTEMS_COMBAT_SYSTEM_H_

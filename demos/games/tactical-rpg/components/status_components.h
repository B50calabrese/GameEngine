#ifndef DEMOS_GAMES_TACTICAL_RPG_COMPONENTS_STATUS_COMPONENTS_H_
#define DEMOS_GAMES_TACTICAL_RPG_COMPONENTS_STATUS_COMPONENTS_H_

#include <string>
#include <vector>
#include <engine/ecs/entity_manager.h>

namespace tactical_rpg {

struct StatusEffectComponent {
  struct Instance {
    std::string name;
    int duration;
    engine::ecs::EntityID effect_entity;
  };
  std::vector<Instance> active_effects;
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_COMPONENTS_STATUS_COMPONENTS_H_

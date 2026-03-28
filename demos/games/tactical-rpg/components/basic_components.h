#ifndef DEMOS_GAMES_TACTICAL_RPG_COMPONENTS_BASIC_COMPONENTS_H_
#define DEMOS_GAMES_TACTICAL_RPG_COMPONENTS_BASIC_COMPONENTS_H_

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <engine/ecs/entity_manager.h>

#include "../game_types.h"

namespace tactical_rpg {

struct GridPositionComponent {
  glm::ivec2 pos;
};

struct IdentityComponent {
  std::string name;
  ClassType class_type;
  bool is_enemy;
};

struct TurnStateComponent {
  int movement_remaining = 0;
  bool action_used = false;
  bool bonus_action_used = false;
  int initiative_roll = 0;
  bool is_downed = false;
};

struct ActionListComponent {
  std::vector<engine::ecs::EntityID> actions;
};

struct EffectsListComponent {
  std::vector<engine::ecs::EntityID> effects;
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_COMPONENTS_BASIC_COMPONENTS_H_

#ifndef DEMOS_GAMES_TACTICAL_RPG_COMPONENTS_H_
#define DEMOS_GAMES_TACTICAL_RPG_COMPONENTS_H_

#include <string>
#include <vector>

#include <engine/ecs/entity_manager.h>
#include <glm/glm.hpp>

#include "game_types.h"

namespace tactical_rpg {

// --- Basic Components ---

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

// --- Grid Components ---

struct TileComponent {
  TerrainType terrain;
  glm::ivec2 grid_pos;
};

struct GridMapComponent {
  static constexpr int kSize = 10;
  engine::ecs::EntityID tiles[kSize][kSize];
};

// --- Action & Effect Components ---

struct ActionDataComponent {
  std::string name;
  ActionType type;
  int range;
  bool is_bonus_action;
  std::string description;
};

struct DamageEffectComponent {
  int dice_size;
  int num_dice;
  int modifier;
};

struct HealEffectComponent {
  int dice_size;
  int num_dice;
  int modifier;
};

// --- Status Components ---

struct StatusEffectComponent {
  struct Instance {
    std::string name;
    int duration;
    engine::ecs::EntityID effect_entity;
  };
  std::vector<Instance> active_effects;
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_COMPONENTS_H_

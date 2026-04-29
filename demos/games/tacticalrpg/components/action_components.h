#ifndef DEMOS_GAMES_TACTICAL_RPG_COMPONENTS_ACTION_COMPONENTS_H_
#define DEMOS_GAMES_TACTICAL_RPG_COMPONENTS_ACTION_COMPONENTS_H_

#include <string>
#include <vector>

#include "../game_types.h"

namespace tactical_rpg {

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

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_COMPONENTS_ACTION_COMPONENTS_H_

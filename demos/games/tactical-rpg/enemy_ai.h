#ifndef DEMOS_GAMES_TACTICAL_RPG_ENEMY_AI_H_
#define DEMOS_GAMES_TACTICAL_RPG_ENEMY_AI_H_

#include <vector>

#include <engine/ecs/registry.h>

#include "game_types.h"

namespace tactical_rpg {

class EnemyAI {
 public:
  static void ProcessTurn(engine::ecs::Registry& registry,
                          engine::ecs::EntityID active);
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_ENEMY_AI_H_

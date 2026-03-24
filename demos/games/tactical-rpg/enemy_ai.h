#ifndef DEMOS_GAMES_TACTICAL_RPG_ENEMY_AI_H_
#define DEMOS_GAMES_TACTICAL_RPG_ENEMY_AI_H_

#include <vector>

#include "game_types.h"

namespace tactical_rpg {

class EnemyAI {
 public:
  static void ProcessTurn(Character* active, std::vector<Character>& party);
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_ENEMY_AI_H_

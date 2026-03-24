#ifndef DEMOS_GAMES_TACTICAL_RPG_TURN_MANAGER_H_
#define DEMOS_GAMES_TACTICAL_RPG_TURN_MANAGER_H_

#include <algorithm>
#include <random>
#include <vector>

#include "game_types.h"

namespace tactical_rpg {

class TurnManager {
 public:
  void RollInitiative(std::vector<Character>& party,
                      std::vector<Character>& enemies);
  void NextTurn();

  void OnTurnStart(Character* character);
  void OnTurnEnd(Character* character);

  Character* GetActiveCharacter() const;
  bool IsBattleOver(const std::vector<Character>& party,
                    const std::vector<Character>& enemies) const;

 private:
  std::vector<Character*> turn_order_;
  int current_index_ = -1;
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_TURN_MANAGER_H_

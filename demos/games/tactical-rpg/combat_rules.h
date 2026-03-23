#ifndef DEMOS_GAMES_TACTICAL_RPG_COMBAT_RULES_H_
#define DEMOS_GAMES_TACTICAL_RPG_COMBAT_RULES_H_

#include <vector>

#include "battle_grid.h"
#include "game_types.h"

namespace tactical_rpg {

class CombatRules {
 public:
  static bool CanMoveTo(const Character& character, const BattleGrid& grid,
                        int x, int y, const std::vector<Character>& all_chars) {
    if (!grid.IsWalkable(x, y)) return false;

    // Occupied by another character
    for (const auto& c : all_chars) {
      if (!c.is_downed && c.grid_pos.x == x && c.grid_pos.y == y) return false;
    }

    int dist =
        std::abs(x - character.grid_pos.x) + std::abs(y - character.grid_pos.y);
    return dist <= character.movement_remaining;
  }

  static bool IsInRange(const Character& actor, const Character& target,
                        int range) {
    int dist = std::abs(actor.grid_pos.x - target.grid_pos.x) +
               std::abs(actor.grid_pos.y - target.grid_pos.y);
    return dist <= range;
  }
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_COMBAT_RULES_H_

#ifndef DEMOS_GAMES_TACTICAL_RPG_COMBAT_RULES_H_
#define DEMOS_GAMES_TACTICAL_RPG_COMBAT_RULES_H_

#include <vector>

#include <engine/ecs/registry.h>

#include "battle_grid.h"
#include "components.h"
#include "game_types.h"

namespace tactical_rpg {

class CombatRules {
 public:
  static bool CanMoveTo(engine::ecs::Registry& registry,
                        engine::ecs::EntityID character_entity, int x, int y) {
    if (!BattleGrid::IsWalkable(registry, x, y)) return false;

    auto& active_grid_pos =
        registry.GetComponent<GridPositionComponent>(character_entity);
    auto& turn_state =
        registry.GetComponent<TurnStateComponent>(character_entity);

    // Occupied by another character
    auto view = registry.GetView<GridPositionComponent, TurnStateComponent>();
    for (auto entity : view) {
      if (entity == character_entity) continue;
      auto& other_grid_pos = registry.GetComponent<GridPositionComponent>(entity);
      auto& other_turn_state = registry.GetComponent<TurnStateComponent>(entity);
      if (!other_turn_state.is_downed && other_grid_pos.pos.x == x &&
          other_grid_pos.pos.y == y) {
        return false;
      }
    }

    int dist = std::abs(x - active_grid_pos.pos.x) +
               std::abs(y - active_grid_pos.pos.y);
    return dist <= turn_state.movement_remaining;
  }

  static bool IsInRange(engine::ecs::Registry& registry,
                        engine::ecs::EntityID actor,
                        engine::ecs::EntityID target, int range) {
    auto& actor_pos = registry.GetComponent<GridPositionComponent>(actor).pos;
    auto& target_pos = registry.GetComponent<GridPositionComponent>(target).pos;

    int dist = std::abs(actor_pos.x - target_pos.x) +
               std::abs(actor_pos.y - target_pos.y);
    return dist <= range;
  }
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_COMBAT_RULES_H_

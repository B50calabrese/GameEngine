#ifndef DEMOS_GAMES_TACTICAL_RPG_TURN_MANAGER_H_
#define DEMOS_GAMES_TACTICAL_RPG_TURN_MANAGER_H_

#include <algorithm>
#include <random>
#include <vector>

#include <engine/ecs/registry.h>

#include "game_types.h"

namespace tactical_rpg {

class TurnManager {
 public:
  void RollInitiative(engine::ecs::Registry& registry);
  void NextTurn(engine::ecs::Registry& registry);

  void OnTurnStart(engine::ecs::Registry& registry,
                   engine::ecs::EntityID entity);
  void OnTurnEnd(engine::ecs::Registry& registry, engine::ecs::EntityID entity);

  engine::ecs::EntityID GetActiveCharacter() const;
  bool IsBattleOver(engine::ecs::Registry& registry) const;

 private:
  std::vector<engine::ecs::EntityID> turn_order_;
  int current_index_ = -1;
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_TURN_MANAGER_H_

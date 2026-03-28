#include "turn_manager.h"

#include <engine/ecs/registry.h>

#include "components.h"

namespace tactical_rpg {

void TurnManager::RollInitiative(engine::ecs::Registry& registry) {
  std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<int> d20(1, 20);

  turn_order_.clear();
  auto view = registry.GetView<IdentityComponent, Stats, TurnStateComponent>();
  for (auto entity : view) {
    auto& stats = registry.GetComponent<Stats>(entity);
    auto& turn_state = registry.GetComponent<TurnStateComponent>(entity);

    if (!turn_state.is_downed) {
      turn_state.initiative_roll = d20(gen) + stats.initiative_mod;
      turn_order_.push_back(entity);
    }
  }

  std::sort(
      turn_order_.begin(), turn_order_.end(),
      [&registry](engine::ecs::EntityID a, engine::ecs::EntityID b) {
        return registry.GetComponent<TurnStateComponent>(a).initiative_roll >
               registry.GetComponent<TurnStateComponent>(b).initiative_roll;
      });

  current_index_ = -1;
}

void TurnManager::OnTurnStart(engine::ecs::Registry& registry,
                              engine::ecs::EntityID entity) {
  auto& stats = registry.GetComponent<Stats>(entity);
  auto& turn_state = registry.GetComponent<TurnStateComponent>(entity);

  // Status effects tick would go here
  if (registry.HasComponent<StatusEffectComponent>(entity)) {
    auto& status = registry.GetComponent<StatusEffectComponent>(entity);
    for (auto it = status.active_effects.begin();
         it != status.active_effects.end();) {
      // Effect logic...
      it->duration--;
      if (it->duration <= 0)
        it = status.active_effects.erase(it);
      else
        ++it;
    }
  }

  // Reset turn state
  turn_state.movement_remaining = stats.speed;
  turn_state.action_used = false;
  turn_state.bonus_action_used = false;
}

void TurnManager::OnTurnEnd(engine::ecs::Registry& registry,
                            engine::ecs::EntityID entity) {
  // End of turn logic
}

void TurnManager::NextTurn(engine::ecs::Registry& registry) {
  if (turn_order_.empty()) return;

  if (current_index_ >= 0) {
    OnTurnEnd(registry, turn_order_[current_index_]);
  }

  current_index_ = (current_index_ + 1) % turn_order_.size();
  auto active = turn_order_[current_index_];

  if (registry.GetComponent<TurnStateComponent>(active).is_downed) {
    NextTurn(registry);
    return;
  }

  OnTurnStart(registry, active);
}

engine::ecs::EntityID TurnManager::GetActiveCharacter() const {
  if (current_index_ < 0 || current_index_ >= (int)turn_order_.size())
    return engine::ecs::INVALID_ENTITY;
  return turn_order_[current_index_];
}

bool TurnManager::IsBattleOver(engine::ecs::Registry& registry) const {
  auto view = registry.GetView<IdentityComponent, TurnStateComponent>();
  bool any_enemy_alive = false;
  bool any_player_alive = false;

  for (auto entity : view) {
    auto& id = registry.GetComponent<IdentityComponent>(entity);
    auto& turn_state = registry.GetComponent<TurnStateComponent>(entity);
    if (!turn_state.is_downed) {
      if (id.is_enemy)
        any_enemy_alive = true;
      else
        any_player_alive = true;
    }
  }

  return !any_enemy_alive || !any_player_alive;
}

}  // namespace tactical_rpg

#include "combat_system.h"

#include <random>

#include <engine/ecs/registry.h>

#include "../components.h"

namespace tactical_rpg {

int CombatSystem::ApplyEffect(engine::ecs::Registry& registry,
                              engine::ecs::EntityID action_entity,
                              engine::ecs::EntityID target_entity) {
  if (!registry.IsAlive(action_entity) || !registry.IsAlive(target_entity))
    return 0;

  auto& target_stats = registry.GetComponent<Stats>(target_entity);
  auto& effects_list =
      registry.GetComponent<EffectsListComponent>(action_entity);

  std::mt19937 gen(std::random_device{}());
  int total_result = 0;

  for (auto effect_entity : effects_list.effects) {
    if (registry.HasComponent<DamageEffectComponent>(effect_entity)) {
      auto& damage =
          registry.GetComponent<DamageEffectComponent>(effect_entity);
      int amt = 0;
      for (int i = 0; i < damage.num_dice; ++i)
        amt += std::uniform_int_distribution<int>(1, damage.dice_size)(gen);
      amt += damage.modifier;
      target_stats.current_hp -= amt;
      total_result -= amt;
    } else if (registry.HasComponent<HealEffectComponent>(effect_entity)) {
      auto& heal = registry.GetComponent<HealEffectComponent>(effect_entity);
      int amt = 0;
      for (int i = 0; i < heal.num_dice; ++i)
        amt += std::uniform_int_distribution<int>(1, heal.dice_size)(gen);
      amt += heal.modifier;
      int actual_heal =
          std::min(target_stats.max_hp - target_stats.current_hp, amt);
      target_stats.current_hp += actual_heal;
      total_result += actual_heal;
    }
  }

  if (target_stats.current_hp <= 0) {
    target_stats.current_hp = 0;
    registry.GetComponent<TurnStateComponent>(target_entity).is_downed = true;
  }

  return total_result;
}

}  // namespace tactical_rpg

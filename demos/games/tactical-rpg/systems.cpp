#include "systems.h"

#include <random>

#include <engine/ecs/components/graphics_components.h>
#include <engine/ecs/components/transform.h>

#include "components.h"

namespace tactical_rpg {

void SyncTransformSystem::Update(engine::ecs::Registry& registry,
                                 float tile_size,
                                 const glm::vec2& grid_offset) {
  auto view = registry.GetView<GridPositionComponent,
                               ::engine::ecs::components::Transform>();
  for (auto entity : view) {
    auto& grid_pos = registry.GetComponent<GridPositionComponent>(entity);
    auto& transform =
        registry.GetComponent<::engine::ecs::components::Transform>(entity);

    transform.position.x =
        grid_offset.x + grid_pos.pos.x * tile_size + tile_size / 2.0f;
    transform.position.y =
        grid_offset.y + grid_pos.pos.y * tile_size + tile_size / 2.0f;
  }
}

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
      int actual_heal = std::min(target_stats.max_hp - target_stats.current_hp, amt);
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

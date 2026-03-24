#include "class_registry.h"

#include <vector>

#include <engine/core/transform.h>
#include <engine/graphics/graphics_components.h>

#include "action_registry.h"
#include "components.h"

namespace tactical_rpg {

void ClassRegistry::RegisterAll() {
  RegisterBarbarian(*this);
  RegisterBard(*this);
  RegisterCleric(*this);
  RegisterDruid(*this);
  RegisterFighter(*this);
  RegisterMonk(*this);
  RegisterPaladin(*this);
  RegisterRanger(*this);
  RegisterRogue(*this);
  RegisterSorcerer(*this);
  RegisterWarlock(*this);
  RegisterWizard(*this);
}

engine::ecs::EntityID ClassRegistry::CreateCharacter(
    engine::ecs::Registry& registry, ClassType type, const std::string& name,
    bool is_enemy) {
  auto entity = registry.CreateEntity();
  const auto& def = definitions_[type];

  registry.AddComponent(entity, IdentityComponent{name, type, is_enemy});
  Stats stats = def.base_stats;
  stats.current_hp = stats.max_hp;
  registry.AddComponent(entity, stats);
  registry.AddComponent(entity, TurnStateComponent{stats.speed, false, false, 0, false});

  ActionListComponent action_list;
  for (const auto& action_name : def.base_actions) {
    auto action_entity =
        ActionRegistry::Get().GetOrCreateAction(registry, action_name);
    if (action_entity != engine::ecs::INVALID_ENTITY) {
      action_list.actions.push_back(action_entity);
    }
  }
  registry.AddComponent(entity, action_list);

  // Engine components
  registry.AddComponent(entity, ::engine::core::TransformComponent());

  return entity;
}

}  // namespace tactical_rpg

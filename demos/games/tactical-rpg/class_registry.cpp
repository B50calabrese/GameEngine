#include "class_registry.h"

#include <vector>

#include <engine/core/transform.h>
#include <engine/graphics/graphics_components.h>

#include "action_registry.h"
#include "components.h"

namespace tactical_rpg {

void ClassRegistry::RegisterAll() {
  // Barbarian
  {
    ClassDefinition def;
    def.type = ClassType::Barbarian;
    def.name = "Barbarian";
    def.base_stats = {16, 14, 16, 8, 10, 8, 12 + 3, 15, 6, 3};
    def.base_actions.push_back("Greataxe");
    def.base_actions.push_back("Rage");
    RegisterClass(def);
  }

  // Fighter
  {
    ClassDefinition def;
    def.type = ClassType::Fighter;
    def.name = "Fighter";
    def.base_stats = {16, 14, 14, 10, 10, 8, 10 + 2, 18, 6, 2};
    def.base_actions.push_back("Longsword");
    def.base_actions.push_back("Second Wind");
    RegisterClass(def);
  }

  // Rogue
  {
    ClassDefinition def;
    def.type = ClassType::Rogue;
    def.name = "Rogue";
    def.base_stats = {8, 16, 14, 12, 10, 12, 8 + 2, 14, 6, 3};
    def.base_actions.push_back("Shortsword");
    def.base_actions.push_back("Cunning Action");
    RegisterClass(def);
  }

  // Wizard
  {
    ClassDefinition def;
    def.type = ClassType::Wizard;
    def.name = "Wizard";
    def.base_stats = {8, 12, 14, 16, 10, 8, 6 + 2, 11, 6, 1};
    def.base_actions.push_back("Dagger");
    def.base_actions.push_back("Firebolt");
    RegisterClass(def);
  }

  // Cleric
  {
    ClassDefinition def;
    def.type = ClassType::Cleric;
    def.name = "Cleric";
    def.base_stats = {14, 10, 14, 10, 16, 12, 8 + 2, 16, 6, 0};
    def.base_actions.push_back("Mace");
    def.base_actions.push_back("Sacred Flame");
    RegisterClass(def);
  }

  // Bard
  {
    ClassDefinition def;
    def.type = ClassType::Bard;
    def.name = "Bard";
    def.base_stats = {8, 14, 12, 12, 10, 16, 8 + 1, 12, 6, 2};
    def.base_actions.push_back("Rapier");
    def.base_actions.push_back("Vicious Mockery");
    RegisterClass(def);
  }

  // Druid
  {
    ClassDefinition def;
    def.type = ClassType::Druid;
    def.name = "Druid";
    def.base_stats = {10, 12, 14, 12, 16, 8, 8 + 2, 14, 6, 1};
    def.base_actions.push_back("Quarterstaff");
    def.base_actions.push_back("Produce Flame");
    RegisterClass(def);
  }

  // Monk
  {
    ClassDefinition def;
    def.type = ClassType::Monk;
    def.name = "Monk";
    def.base_stats = {10, 16, 14, 10, 16, 8, 8 + 2, 16, 8, 3};
    def.base_actions.push_back("Unarmed");
    def.base_actions.push_back("Martial Arts");
    RegisterClass(def);
  }

  // Paladin
  {
    ClassDefinition def;
    def.type = ClassType::Paladin;
    def.name = "Paladin";
    def.base_stats = {16, 10, 14, 8, 10, 16, 10 + 2, 18, 6, 0};
    def.base_actions.push_back("Warhammer");
    def.base_actions.push_back("Lay on Hands");
    RegisterClass(def);
  }

  // Ranger
  {
    ClassDefinition def;
    def.type = ClassType::Ranger;
    def.name = "Ranger";
    def.base_stats = {12, 16, 14, 8, 14, 10, 10 + 2, 15, 6, 3};
    def.base_actions.push_back("Longbow");
    def.base_actions.push_back("Hunter's Mark");
    RegisterClass(def);
  }

  // Sorcerer
  {
    ClassDefinition def;
    def.type = ClassType::Sorcerer;
    def.name = "Sorcerer";
    def.base_stats = {8, 12, 14, 10, 10, 16, 6 + 2, 11, 6, 1};
    def.base_actions.push_back("Quarterstaff");
    def.base_actions.push_back("Firebolt");
    RegisterClass(def);
  }

  // Warlock
  {
    ClassDefinition def;
    def.type = ClassType::Warlock;
    def.name = "Warlock";
    def.base_stats = {8, 12, 14, 10, 10, 16, 8 + 2, 12, 6, 1};
    def.base_actions.push_back("Dagger");
    def.base_actions.push_back("Eldritch Blast");
    RegisterClass(def);
  }
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

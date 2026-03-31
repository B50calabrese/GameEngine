#include "action_registry.h"

#include <engine/ecs/registry.h>

#include "components.h"

namespace tactical_rpg {

void ActionRegistry::RegisterAll() {
  // Attacks
  RegisterAction({"Greataxe",
                  ActionType::Attack,
                  1,
                  false,
                  "Heavy axe swing.",
                  {{true, 12, 1, 3}}});
  RegisterAction({"Longsword",
                  ActionType::Attack,
                  1,
                  false,
                  "Versatile blade strike.",
                  {{true, 10, 1, 3}}});
  RegisterAction({"Shortsword",
                  ActionType::Attack,
                  1,
                  false,
                  "Quick piercing strike.",
                  {{true, 6, 1, 3}}});
  RegisterAction({"Dagger",
                  ActionType::Attack,
                  1,
                  false,
                  "Small blade strike.",
                  {{true, 4, 1, 1}}});
  RegisterAction({"Mace",
                  ActionType::Attack,
                  1,
                  false,
                  "Blunt strike.",
                  {{true, 6, 1, 2}}});
  RegisterAction({"Rapier",
                  ActionType::Attack,
                  1,
                  false,
                  "Elegant strike.",
                  {{true, 8, 1, 2}}});
  RegisterAction({"Quarterstaff",
                  ActionType::Attack,
                  1,
                  false,
                  "Wooden strike.",
                  {{true, 6, 1, 0}}});
  RegisterAction({"Unarmed",
                  ActionType::Attack,
                  1,
                  false,
                  "Bare hands.",
                  {{true, 8, 1, 3}}});
  RegisterAction({"Martial Arts",
                  ActionType::Attack,
                  1,
                  true,
                  "Quick kick.",
                  {{true, 4, 1, 3}}});
  RegisterAction({"Warhammer",
                  ActionType::Attack,
                  1,
                  false,
                  "Heavy strike.",
                  {{true, 8, 1, 3}}});
  RegisterAction({"Longbow",
                  ActionType::Attack,
                  30,
                  false,
                  "Ranged shot.",
                  {{true, 8, 1, 3}}});

  // Spells
  RegisterAction({"Firebolt",
                  ActionType::Spell,
                  24,
                  false,
                  "Hurl fire.",
                  {{true, 10, 1, 0}}});
  RegisterAction({"Sacred Flame",
                  ActionType::Spell,
                  12,
                  false,
                  "Divine fire.",
                  {{true, 8, 1, 0}}});
  RegisterAction({"Vicious Mockery",
                  ActionType::Spell,
                  12,
                  false,
                  "Harmful insult.",
                  {{true, 4, 1, 0}}});
  RegisterAction({"Produce Flame",
                  ActionType::Spell,
                  6,
                  false,
                  "Flickering flame.",
                  {{true, 8, 1, 0}}});
  RegisterAction({"Eldritch Blast",
                  ActionType::Spell,
                  24,
                  false,
                  "Energy beam.",
                  {{true, 10, 1, 0}}});
  RegisterAction({"Ray of Frost",
                  ActionType::Spell,
                  12,
                  false,
                  "Frigid beam.",
                  {{true, 8, 1, 0}}});

  // Abilities
  RegisterAction({"Rage", ActionType::Ability, 0, true, "Enter rage.", {}});
  RegisterAction({"Second Wind",
                  ActionType::Ability,
                  0,
                  true,
                  "Self heal.",
                  {{false, 10, 1, 2}}});
  RegisterAction(
      {"Cunning Action", ActionType::Ability, 0, true, "Extra movement.", {}});
  RegisterAction({"Lay on Hands",
                  ActionType::Ability,
                  1,
                  false,
                  "Heal touch.",
                  {{false, 0, 0, 5}}});
  RegisterAction(
      {"Hunter's Mark", ActionType::Spell, 18, true, "Mark target.", {}});
}

engine::ecs::EntityID ActionRegistry::GetOrCreateAction(
    engine::ecs::Registry& registry, const std::string& name) {
  if (action_cache_.count(name)) return action_cache_[name];

  auto it = actions_.find(name);
  if (it == actions_.end()) return engine::ecs::kInvalidEntity;

  const auto& raw = it->second;
  auto action_entity = registry.CreateEntity();
  registry.AddComponent(
      action_entity, ActionDataComponent{raw.name, raw.type, raw.range,
                                         raw.is_bonus_action, raw.description});

  EffectsListComponent effects_list;
  for (const auto& raw_effect : raw.effects) {
    auto effect_entity = registry.CreateEntity();
    if (raw_effect.is_damage) {
      registry.AddComponent(
          effect_entity,
          DamageEffectComponent{raw_effect.dice_size, raw_effect.num_dice,
                                raw_effect.modifier});
    } else {
      registry.AddComponent(
          effect_entity,
          HealEffectComponent{raw_effect.dice_size, raw_effect.num_dice,
                              raw_effect.modifier});
    }
    effects_list.effects.push_back(effect_entity);
  }
  registry.AddComponent(action_entity, effects_list);

  action_cache_[name] = action_entity;
  return action_entity;
}

}  // namespace tactical_rpg

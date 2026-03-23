#include "action_registry.h"
#include "effect.h"

namespace tactical_rpg {

void ActionRegistry::RegisterAll() {
  // Attacks
  RegisterAction({"Greataxe", ActionType::Attack, 1, false, "Heavy axe swing.", {std::make_shared<DamageEffect>(12, 1, 3)}});
  RegisterAction({"Longsword", ActionType::Attack, 1, false, "Versatile blade strike.", {std::make_shared<DamageEffect>(10, 1, 3)}});
  RegisterAction({"Shortsword", ActionType::Attack, 1, false, "Quick piercing strike.", {std::make_shared<DamageEffect>(6, 1, 3)}});
  RegisterAction({"Dagger", ActionType::Attack, 1, false, "Small blade strike.", {std::make_shared<DamageEffect>(4, 1, 1)}});
  RegisterAction({"Mace", ActionType::Attack, 1, false, "Blunt strike.", {std::make_shared<DamageEffect>(6, 1, 2)}});
  RegisterAction({"Rapier", ActionType::Attack, 1, false, "Elegant strike.", {std::make_shared<DamageEffect>(8, 1, 2)}});
  RegisterAction({"Quarterstaff", ActionType::Attack, 1, false, "Wooden strike.", {std::make_shared<DamageEffect>(6, 1, 0)}});
  RegisterAction({"Unarmed", ActionType::Attack, 1, false, "Bare hands.", {std::make_shared<DamageEffect>(8, 1, 3)}});
  RegisterAction({"Martial Arts", ActionType::Attack, 1, true, "Quick kick.", {std::make_shared<DamageEffect>(4, 1, 3)}});
  RegisterAction({"Warhammer", ActionType::Attack, 1, false, "Heavy strike.", {std::make_shared<DamageEffect>(8, 1, 3)}});
  RegisterAction({"Longbow", ActionType::Attack, 30, false, "Ranged shot.", {std::make_shared<DamageEffect>(8, 1, 3)}});

  // Spells
  RegisterAction({"Firebolt", ActionType::Spell, 24, false, "Hurl fire.", {std::make_shared<DamageEffect>(10, 1, 0)}});
  RegisterAction({"Sacred Flame", ActionType::Spell, 12, false, "Divine fire.", {std::make_shared<DamageEffect>(8, 1, 0)}});
  RegisterAction({"Vicious Mockery", ActionType::Spell, 12, false, "Harmful insult.", {std::make_shared<DamageEffect>(4, 1, 0)}});
  RegisterAction({"Produce Flame", ActionType::Spell, 6, false, "Flickering flame.", {std::make_shared<DamageEffect>(8, 1, 0)}});
  RegisterAction({"Eldritch Blast", ActionType::Spell, 24, false, "Energy beam.", {std::make_shared<DamageEffect>(10, 1, 0)}});
  RegisterAction({"Ray of Frost", ActionType::Spell, 12, false, "Frigid beam.", {std::make_shared<DamageEffect>(8, 1, 0)}});

  // Abilities
  RegisterAction({"Rage", ActionType::Ability, 0, true, "Enter rage.", {}});
  RegisterAction({"Second Wind", ActionType::Ability, 0, true, "Self heal.", {std::make_shared<HealEffect>(10, 1, 2)}});
  RegisterAction({"Cunning Action", ActionType::Ability, 0, true, "Extra movement.", {}});
  RegisterAction({"Lay on Hands", ActionType::Ability, 1, false, "Heal touch.", {std::make_shared<HealEffect>(0, 0, 5)}});
  RegisterAction({"Hunter's Mark", ActionType::Spell, 18, true, "Mark target.", {}});
}

}  // namespace tactical_rpg

#include "class_registry.h"

#include <vector>

namespace tactical_rpg {

void ClassRegistry::RegisterAll() {
  // Barbarian
  {
    ClassDefinition def;
    def.type = ClassType::Barbarian;
    def.name = "Barbarian";
    def.base_stats = {16, 14, 16, 8, 10, 8, 0, 0, 0, 6, 2};
    def.base_stats.max_hp = 12 + Stats::GetModifier(16);
    def.base_stats.ac = 10 + Stats::GetModifier(14) + Stats::GetModifier(16);
    def.base_actions.push_back({"Greataxe", ActionType::Attack, 1, 12, 1, 3,
                                false, "Heavy axe swing."});
    def.base_actions.push_back(
        {"Rage", ActionType::Ability, 0, 0, 0, 0, true, "Enter rage."});
    RegisterClass(def);
  }

  // Fighter
  {
    ClassDefinition def;
    def.type = ClassType::Fighter;
    def.name = "Fighter";
    def.base_stats = {16, 14, 14, 10, 10, 8, 0, 0, 0, 6, 2};
    def.base_stats.max_hp = 10 + Stats::GetModifier(14);
    def.base_stats.ac = 18;
    def.base_actions.push_back({"Longsword", ActionType::Attack, 1, 10, 1, 3,
                                false, "Versatile blade strike."});
    def.base_actions.push_back(
        {"Second Wind", ActionType::Ability, 0, 0, 0, 0, true, "Self heal."});
    RegisterClass(def);
  }

  // Rogue
  {
    ClassDefinition def;
    def.type = ClassType::Rogue;
    def.name = "Rogue";
    def.base_stats = {8, 16, 14, 12, 10, 12, 0, 0, 0, 6, 3};
    def.base_stats.max_hp = 8 + Stats::GetModifier(14);
    def.base_stats.ac = 11 + Stats::GetModifier(16);
    def.base_actions.push_back({"Shortsword", ActionType::Attack, 1, 6, 1, 3,
                                false, "Quick piercing strike."});
    def.base_actions.push_back({"Cunning Action", ActionType::Ability, 0, 0, 0,
                                0, true, "Extra movement."});
    RegisterClass(def);
  }

  // Wizard
  {
    ClassDefinition def;
    def.type = ClassType::Wizard;
    def.name = "Wizard";
    def.base_stats = {8, 12, 14, 16, 10, 8, 0, 0, 0, 6, 1};
    def.base_stats.max_hp = 6 + Stats::GetModifier(14);
    def.base_stats.ac = 10 + Stats::GetModifier(12);
    def.base_actions.push_back({"Dagger", ActionType::Attack, 1, 4, 1, 1, false,
                                "Small blade strike."});
    def.base_actions.push_back(
        {"Firebolt", ActionType::Spell, 24, 10, 1, 0, false, "Hurl fire."});
    RegisterClass(def);
  }

  // Cleric
  {
    ClassDefinition def;
    def.type = ClassType::Cleric;
    def.name = "Cleric";
    def.base_stats = {14, 10, 14, 10, 16, 12, 0, 0, 0, 6, 0};
    def.base_stats.max_hp = 8 + Stats::GetModifier(14);
    def.base_stats.ac = 16;
    def.base_actions.push_back(
        {"Mace", ActionType::Attack, 1, 6, 1, 2, false, "Blunt strike."});
    def.base_actions.push_back({"Sacred Flame", ActionType::Spell, 12, 8, 1, 0,
                                false, "Divine fire."});
    RegisterClass(def);
  }

  // Bard
  {
    ClassDefinition def;
    def.type = ClassType::Bard;
    def.name = "Bard";
    def.base_stats = {8, 14, 12, 12, 10, 16, 0, 0, 0, 6, 2};
    def.base_stats.max_hp = 8 + Stats::GetModifier(12);
    def.base_stats.ac = 11 + Stats::GetModifier(14);
    def.base_actions.push_back(
        {"Rapier", ActionType::Attack, 1, 8, 1, 2, false, "Elegant strike."});
    def.base_actions.push_back({"Vicious Mockery", ActionType::Spell, 12, 4, 1,
                                0, false, "Harmful insult."});
    RegisterClass(def);
  }

  // Druid
  {
    ClassDefinition def;
    def.type = ClassType::Druid;
    def.name = "Druid";
    def.base_stats = {10, 12, 14, 12, 16, 8, 0, 0, 0, 6, 1};
    def.base_stats.max_hp = 8 + Stats::GetModifier(14);
    def.base_stats.ac = 13 + Stats::GetModifier(12);
    def.base_actions.push_back({"Quarterstaff", ActionType::Attack, 1, 6, 1, 0,
                                false, "Wooden strike."});
    def.base_actions.push_back({"Produce Flame", ActionType::Spell, 6, 8, 1, 0,
                                false, "Flickering flame."});
    RegisterClass(def);
  }

  // Monk
  {
    ClassDefinition def;
    def.type = ClassType::Monk;
    def.name = "Monk";
    def.base_stats = {10, 16, 14, 10, 16, 8, 0, 0, 0, 8, 3};
    def.base_stats.max_hp = 8 + Stats::GetModifier(14);
    def.base_stats.ac = 10 + Stats::GetModifier(16) + Stats::GetModifier(16);
    def.base_actions.push_back(
        {"Unarmed", ActionType::Attack, 1, 8, 1, 3, false, "Bare hands."});
    def.base_actions.push_back(
        {"Martial Arts", ActionType::Attack, 1, 4, 1, 3, true, "Quick kick."});
    RegisterClass(def);
  }

  // Paladin
  {
    ClassDefinition def;
    def.type = ClassType::Paladin;
    def.name = "Paladin";
    def.base_stats = {16, 10, 14, 8, 10, 16, 0, 0, 0, 6, 0};
    def.base_stats.max_hp = 10 + Stats::GetModifier(14);
    def.base_stats.ac = 18;
    def.base_actions.push_back(
        {"Warhammer", ActionType::Attack, 1, 8, 1, 3, false, "Heavy strike."});
    def.base_actions.push_back({"Lay on Hands", ActionType::Ability, 1, 0, 0, 5,
                                false, "Heal touch."});
    RegisterClass(def);
  }

  // Ranger
  {
    ClassDefinition def;
    def.type = ClassType::Ranger;
    def.name = "Ranger";
    def.base_stats = {12, 16, 14, 8, 14, 10, 0, 0, 0, 6, 3};
    def.base_stats.max_hp = 10 + Stats::GetModifier(14);
    def.base_stats.ac = 14 + Stats::GetModifier(16);
    def.base_actions.push_back(
        {"Longbow", ActionType::Attack, 30, 8, 1, 3, false, "Ranged shot."});
    def.base_actions.push_back({"Hunter's Mark", ActionType::Spell, 18, 0, 0, 0,
                                true, "Mark target."});
    RegisterClass(def);
  }

  // Sorcerer
  {
    ClassDefinition def;
    def.type = ClassType::Sorcerer;
    def.name = "Sorcerer";
    def.base_stats = {8, 12, 14, 10, 10, 16, 0, 0, 0, 6, 1};
    def.base_stats.max_hp = 6 + Stats::GetModifier(14);
    def.base_stats.ac = 10 + Stats::GetModifier(12);
    def.base_actions.push_back({"Quarterstaff", ActionType::Attack, 1, 4, 1, -1,
                                false, "Wooden strike."});
    def.base_actions.push_back(
        {"Firebolt", ActionType::Spell, 24, 10, 1, 0, false, "Hurl fire."});
    RegisterClass(def);
  }

  // Warlock
  {
    ClassDefinition def;
    def.type = ClassType::Warlock;
    def.name = "Warlock";
    def.base_stats = {8, 12, 14, 10, 10, 16, 0, 0, 0, 6, 1};
    def.base_stats.max_hp = 8 + Stats::GetModifier(14);
    def.base_stats.ac = 11 + Stats::GetModifier(12);
    def.base_actions.push_back(
        {"Dagger", ActionType::Attack, 1, 4, 1, 1, false, "Small blade."});
    def.base_actions.push_back({"Eldritch Blast", ActionType::Spell, 24, 10, 1,
                                0, false, "Energy beam."});
    RegisterClass(def);
  }
}

}  // namespace tactical_rpg

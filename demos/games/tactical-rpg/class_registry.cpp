#include "class_registry.h"
#include "action_registry.h"
#include <vector>

namespace tactical_rpg {

void ClassRegistry::RegisterAll() {
  auto& ar = ActionRegistry::Get();

  // Barbarian
  {
    ClassDefinition def;
    def.type = ClassType::Barbarian;
    def.name = "Barbarian";
    def.base_stats = {16, 14, 16, 8, 10, 8, 12 + 3, 15, 6, 3}; // str, dex, con, int, wis, cha, hp, ac, spd, init
    def.base_actions.push_back(ar.GetAction("Greataxe"));
    def.base_actions.push_back(ar.GetAction("Rage"));
    RegisterClass(def);
  }

  // Fighter
  {
    ClassDefinition def;
    def.type = ClassType::Fighter;
    def.name = "Fighter";
    def.base_stats = {16, 14, 14, 10, 10, 8, 10 + 2, 18, 6, 2};
    def.base_actions.push_back(ar.GetAction("Longsword"));
    def.base_actions.push_back(ar.GetAction("Second Wind"));
    RegisterClass(def);
  }

  // Rogue
  {
    ClassDefinition def;
    def.type = ClassType::Rogue;
    def.name = "Rogue";
    def.base_stats = {8, 16, 14, 12, 10, 12, 8 + 2, 14, 6, 3};
    def.base_actions.push_back(ar.GetAction("Shortsword"));
    def.base_actions.push_back(ar.GetAction("Cunning Action"));
    RegisterClass(def);
  }

  // Wizard
  {
    ClassDefinition def;
    def.type = ClassType::Wizard;
    def.name = "Wizard";
    def.base_stats = {8, 12, 14, 16, 10, 8, 6 + 2, 11, 6, 1};
    def.base_actions.push_back(ar.GetAction("Dagger"));
    def.base_actions.push_back(ar.GetAction("Firebolt"));
    RegisterClass(def);
  }

  // Cleric
  {
    ClassDefinition def;
    def.type = ClassType::Cleric;
    def.name = "Cleric";
    def.base_stats = {14, 10, 14, 10, 16, 12, 8 + 2, 16, 6, 0};
    def.base_actions.push_back(ar.GetAction("Mace"));
    def.base_actions.push_back(ar.GetAction("Sacred Flame"));
    RegisterClass(def);
  }

  // Bard
  {
    ClassDefinition def;
    def.type = ClassType::Bard;
    def.name = "Bard";
    def.base_stats = {8, 14, 12, 12, 10, 16, 8 + 1, 12, 6, 2};
    def.base_actions.push_back(ar.GetAction("Rapier"));
    def.base_actions.push_back(ar.GetAction("Vicious Mockery"));
    RegisterClass(def);
  }

  // Druid
  {
    ClassDefinition def;
    def.type = ClassType::Druid;
    def.name = "Druid";
    def.base_stats = {10, 12, 14, 12, 16, 8, 8 + 2, 14, 6, 1};
    def.base_actions.push_back(ar.GetAction("Quarterstaff"));
    def.base_actions.push_back(ar.GetAction("Produce Flame"));
    RegisterClass(def);
  }

  // Monk
  {
    ClassDefinition def;
    def.type = ClassType::Monk;
    def.name = "Monk";
    def.base_stats = {10, 16, 14, 10, 16, 8, 8 + 2, 16, 8, 3};
    def.base_actions.push_back(ar.GetAction("Unarmed"));
    def.base_actions.push_back(ar.GetAction("Martial Arts"));
    RegisterClass(def);
  }

  // Paladin
  {
    ClassDefinition def;
    def.type = ClassType::Paladin;
    def.name = "Paladin";
    def.base_stats = {16, 10, 14, 8, 10, 16, 10 + 2, 18, 6, 0};
    def.base_actions.push_back(ar.GetAction("Warhammer"));
    def.base_actions.push_back(ar.GetAction("Lay on Hands"));
    RegisterClass(def);
  }

  // Ranger
  {
    ClassDefinition def;
    def.type = ClassType::Ranger;
    def.name = "Ranger";
    def.base_stats = {12, 16, 14, 8, 14, 10, 10 + 2, 15, 6, 3};
    def.base_actions.push_back(ar.GetAction("Longbow"));
    def.base_actions.push_back(ar.GetAction("Hunter's Mark"));
    RegisterClass(def);
  }

  // Sorcerer
  {
    ClassDefinition def;
    def.type = ClassType::Sorcerer;
    def.name = "Sorcerer";
    def.base_stats = {8, 12, 14, 10, 10, 16, 6 + 2, 11, 6, 1};
    def.base_actions.push_back(ar.GetAction("Quarterstaff"));
    def.base_actions.push_back(ar.GetAction("Firebolt"));
    RegisterClass(def);
  }

  // Warlock
  {
    ClassDefinition def;
    def.type = ClassType::Warlock;
    def.name = "Warlock";
    def.base_stats = {8, 12, 14, 10, 10, 16, 8 + 2, 12, 6, 1};
    def.base_actions.push_back(ar.GetAction("Dagger"));
    def.base_actions.push_back(ar.GetAction("Eldritch Blast"));
    RegisterClass(def);
  }
}

} // namespace tactical_rpg

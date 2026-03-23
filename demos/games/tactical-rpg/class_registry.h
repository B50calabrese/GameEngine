#ifndef DEMOS_GAMES_TACTICAL_RPG_CLASS_REGISTRY_H_
#define DEMOS_GAMES_TACTICAL_RPG_CLASS_REGISTRY_H_

#include <map>
#include <memory>

#include "game_types.h"

namespace tactical_rpg {

class ClassRegistry {
 public:
  static ClassRegistry& Get() {
    static ClassRegistry instance;
    return instance;
  }

  Character CreateCharacter(ClassType type, const std::string& name,
                            bool is_enemy = false) {
    Character c;
    c.name = name;
    c.class_type = type;
    c.is_enemy = is_enemy;
    c.is_downed = false;
    c.stats = GetBaseStats(type);
    c.stats.current_hp = c.stats.max_hp;
    c.actions = GetBaseActions(type);
    return c;
  }

  Stats GetBaseStats(ClassType type) {
    Stats s;
    // Default stats
    s.str = 10;
    s.dex = 10;
    s.con = 10;
    s.intel = 10;
    s.wis = 10;
    s.cha = 10;
    s.speed = 6;  // Standard movement in squares (30ft / 5ft per square)

    switch (type) {
      case ClassType::Barbarian:
        s.str = 16;
        s.dex = 14;
        s.con = 16;
        s.intel = 8;
        s.wis = 10;
        s.cha = 8;
        s.max_hp = 12 + Stats::GetModifier(s.con);
        s.ac = 10 + Stats::GetModifier(s.dex) + Stats::GetModifier(s.con);
        break;
      case ClassType::Bard:
        s.str = 8;
        s.dex = 14;
        s.con = 12;
        s.intel = 12;
        s.wis = 10;
        s.cha = 16;
        s.max_hp = 8 + Stats::GetModifier(s.con);
        s.ac = 11 + Stats::GetModifier(s.dex);  // Leather
        break;
      case ClassType::Cleric:
        s.str = 14;
        s.dex = 10;
        s.con = 14;
        s.intel = 10;
        s.wis = 16;
        s.cha = 12;
        s.max_hp = 8 + Stats::GetModifier(s.con);
        s.ac = 16;  // Scale mail + Shield
        break;
      case ClassType::Druid:
        s.str = 10;
        s.dex = 12;
        s.con = 14;
        s.intel = 12;
        s.wis = 16;
        s.cha = 8;
        s.max_hp = 8 + Stats::GetModifier(s.con);
        s.ac = 13 + Stats::GetModifier(s.dex);  // Hide
        break;
      case ClassType::Fighter:
        s.str = 16;
        s.dex = 14;
        s.con = 14;
        s.intel = 10;
        s.wis = 10;
        s.cha = 8;
        s.max_hp = 10 + Stats::GetModifier(s.con);
        s.ac = 18;  // Chain mail + Shield
        break;
      case ClassType::Monk:
        s.str = 10;
        s.dex = 16;
        s.con = 14;
        s.intel = 10;
        s.wis = 16;
        s.cha = 8;
        s.max_hp = 8 + Stats::GetModifier(s.con);
        s.ac = 10 + Stats::GetModifier(s.dex) + Stats::GetModifier(s.wis);
        s.speed = 8;  // Extra movement
        break;
      case ClassType::Paladin:
        s.str = 16;
        s.dex = 10;
        s.con = 14;
        s.intel = 8;
        s.wis = 10;
        s.cha = 16;
        s.max_hp = 10 + Stats::GetModifier(s.con);
        s.ac = 18;  // Chain mail + Shield
        break;
      case ClassType::Ranger:
        s.str = 12;
        s.dex = 16;
        s.con = 14;
        s.intel = 8;
        s.wis = 14;
        s.cha = 10;
        s.max_hp = 10 + Stats::GetModifier(s.con);
        s.ac = 14 + Stats::GetModifier(s.dex);  // Scale mail
        break;
      case ClassType::Rogue:
        s.str = 8;
        s.dex = 16;
        s.con = 14;
        s.intel = 12;
        s.wis = 10;
        s.cha = 12;
        s.max_hp = 8 + Stats::GetModifier(s.con);
        s.ac = 11 + Stats::GetModifier(s.dex);  // Leather
        break;
      case ClassType::Sorcerer:
        s.str = 8;
        s.dex = 12;
        s.con = 14;
        s.intel = 10;
        s.wis = 10;
        s.cha = 16;
        s.max_hp = 6 + Stats::GetModifier(s.con);
        s.ac = 10 + Stats::GetModifier(s.dex);
        break;
      case ClassType::Warlock:
        s.str = 8;
        s.dex = 12;
        s.con = 14;
        s.intel = 10;
        s.wis = 10;
        s.cha = 16;
        s.max_hp = 8 + Stats::GetModifier(s.con);
        s.ac = 11 + Stats::GetModifier(s.dex);  // Leather
        break;
      case ClassType::Wizard:
        s.str = 8;
        s.dex = 12;
        s.con = 14;
        s.intel = 16;
        s.wis = 10;
        s.cha = 8;
        s.max_hp = 6 + Stats::GetModifier(s.con);
        s.ac = 10 + Stats::GetModifier(s.dex);
        break;
    }
    s.initiative_mod = Stats::GetModifier(s.dex);
    return s;
  }

  std::vector<Action> GetBaseActions(ClassType type) {
    std::vector<Action> actions;
    // Common weapon attack (can be refined later)
    Action weapon_attack = {"Weapon Attack", ActionType::Attack,     1, 8, 1, 0,
                            false,           "A basic melee attack."};

    switch (type) {
      case ClassType::Barbarian:
        weapon_attack.damage_dice_sides = 12;
        weapon_attack.damage_modifier = 3;
        actions.push_back(weapon_attack);
        actions.push_back({"Rage", ActionType::Ability, 0, 0, 0, 0, true,
                           "Enter a primal rage (bonus action)."});
        break;
      case ClassType::Bard:
        weapon_attack.damage_dice_sides = 6;
        actions.push_back(weapon_attack);
        actions.push_back({"Vicious Mockery", ActionType::Spell, 12, 4, 1, 0,
                           false, "Insult an enemy to deal damage."});
        break;
      case ClassType::Cleric:
        weapon_attack.damage_dice_sides = 6;
        actions.push_back(weapon_attack);
        actions.push_back({"Sacred Flame", ActionType::Spell, 12, 8, 1, 0,
                           false, "Divine fire strikes from above."});
        break;
      case ClassType::Druid:
        weapon_attack.damage_dice_sides = 4;
        actions.push_back(weapon_attack);
        actions.push_back({"Produce Flame", ActionType::Spell, 6, 8, 1, 0,
                           false, "Flickering flame in your hand."});
        break;
      case ClassType::Fighter:
        weapon_attack.damage_dice_sides = 10;
        weapon_attack.damage_modifier = 3;
        actions.push_back(weapon_attack);
        actions.push_back({"Second Wind", ActionType::Ability, 0, 0, 0, 0, true,
                           "Heal yourself as a bonus action."});
        break;
      case ClassType::Monk:
        weapon_attack.damage_dice_sides = 8;
        actions.push_back(weapon_attack);
        actions.push_back({"Unarmed Strike", ActionType::Attack, 1, 4, 1, 3,
                           true, "Strike with your fists as a bonus action."});
        break;
      case ClassType::Paladin:
        weapon_attack.damage_dice_sides = 10;
        weapon_attack.damage_modifier = 3;
        actions.push_back(weapon_attack);
        actions.push_back({"Lay on Hands", ActionType::Ability, 1, 0, 0, 5,
                           false, "Heal a touched creature."});
        break;
      case ClassType::Ranger:
        weapon_attack.name = "Longbow";
        weapon_attack.range = 30;
        weapon_attack.damage_dice_sides = 8;
        actions.push_back(weapon_attack);
        actions.push_back({"Hunter's Mark", ActionType::Spell, 18, 0, 0, 0,
                           true, "Mark an enemy for extra damage."});
        break;
      case ClassType::Rogue:
        weapon_attack.damage_dice_sides = 6;
        weapon_attack.damage_modifier = 3;
        actions.push_back(weapon_attack);
        actions.push_back({"Cunning Action", ActionType::Ability, 0, 0, 0, 0,
                           true, "Dash or Disengage as a bonus action."});
        break;
      case ClassType::Sorcerer:
        weapon_attack.damage_dice_sides = 4;
        actions.push_back(weapon_attack);
        actions.push_back({"Firebolt", ActionType::Spell, 24, 10, 1, 0, false,
                           "Hurl a mote of fire."});
        break;
      case ClassType::Warlock:
        weapon_attack.damage_dice_sides = 4;
        actions.push_back(weapon_attack);
        actions.push_back({"Eldritch Blast", ActionType::Spell, 24, 10, 1, 0,
                           false, "A beam of crackling energy."});
        break;
      case ClassType::Wizard:
        weapon_attack.damage_dice_sides = 4;
        actions.push_back(weapon_attack);
        actions.push_back({"Ray of Frost", ActionType::Spell, 12, 8, 1, 0,
                           false, "A frigid beam of blue-white light."});
        break;
    }
    return actions;
  }

 private:
  ClassRegistry() = default;
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_CLASS_REGISTRY_H_

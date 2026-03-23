#ifndef DEMOS_GAMES_TACTICAL_RPG_GAME_TYPES_H_
#define DEMOS_GAMES_TACTICAL_RPG_GAME_TYPES_H_

#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace tactical_rpg {

class Effect;

enum class ClassType {
  Barbarian,
  Bard,
  Cleric,
  Druid,
  Fighter,
  Monk,
  Paladin,
  Ranger,
  Rogue,
  Sorcerer,
  Warlock,
  Wizard
};

inline std::string ClassTypeToString(ClassType type) {
  switch (type) {
    case ClassType::Barbarian:
      return "Barbarian";
    case ClassType::Bard:
      return "Bard";
    case ClassType::Cleric:
      return "Cleric";
    case ClassType::Druid:
      return "Druid";
    case ClassType::Fighter:
      return "Fighter";
    case ClassType::Monk:
      return "Monk";
    case ClassType::Paladin:
      return "Paladin";
    case ClassType::Ranger:
      return "Ranger";
    case ClassType::Rogue:
      return "Rogue";
    case ClassType::Sorcerer:
      return "Sorcerer";
    case ClassType::Warlock:
      return "Warlock";
    case ClassType::Wizard:
      return "Wizard";
    default:
      return "Unknown";
  }
}

struct Stats {
  int str, dex, con, intel, wis, cha;
  int max_hp;
  int current_hp;
  int ac;
  int speed;
  int initiative_mod;

  static int GetModifier(int score) { return (score - 10) / 2; }
};

enum class ActionType { Attack, Spell, Ability, Item };

struct Action {
  std::string name;
  ActionType type;
  int range;
  bool is_bonus_action;
  std::string description;
  std::vector<std::shared_ptr<Effect>> effects;
};

struct Character {
  std::string name;
  ClassType class_type;
  Stats stats;
  std::vector<Action> actions;
  bool is_enemy = false;
  glm::ivec2 grid_pos = {0, 0};
  bool is_downed = false;

  // Turn state
  int movement_remaining = 0;
  bool action_used = false;
  bool bonus_action_used = false;
  int initiative_roll = 0;
};

enum class TerrainType {
  Normal,
  Slow,       // e.g., Web
  Damage,     // e.g., Lava
  Impassible  // e.g., Wall
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_GAME_TYPES_H_

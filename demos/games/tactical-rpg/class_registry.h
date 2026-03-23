#ifndef DEMOS_GAMES_TACTICAL_RPG_CLASS_REGISTRY_H_
#define DEMOS_GAMES_TACTICAL_RPG_CLASS_REGISTRY_H_

#include <map>
#include <memory>
#include <string>

#include "game_types.h"

namespace tactical_rpg {

struct ClassDefinition {
  ClassType type;
  std::string name;
  Stats base_stats;
  std::vector<Action> base_actions;
};

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

    const auto& def = definitions_[type];
    c.stats = def.base_stats;
    c.stats.current_hp = c.stats.max_hp;
    c.actions = def.base_actions;
    return c;
  }

 private:
  ClassRegistry() { RegisterAll(); }

  void RegisterAll();
  void RegisterClass(const ClassDefinition& def) {
    definitions_[def.type] = def;
  }

  std::map<ClassType, ClassDefinition> definitions_;
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_CLASS_REGISTRY_H_

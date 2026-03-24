#ifndef DEMOS_GAMES_TACTICAL_RPG_CLASS_REGISTRY_H_
#define DEMOS_GAMES_TACTICAL_RPG_CLASS_REGISTRY_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <engine/ecs/registry.h>
#include "game_types.h"

namespace tactical_rpg {

struct ClassDefinition {
  ClassType type;
  std::string name;
  Stats base_stats;
  std::vector<std::string> base_actions;
};

class ClassRegistry {
 public:
  static ClassRegistry& Get() {
    static ClassRegistry instance;
    return instance;
  }

  engine::ecs::EntityID CreateCharacter(engine::ecs::Registry& registry,
                                        ClassType type, const std::string& name,
                                        bool is_enemy = false);

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

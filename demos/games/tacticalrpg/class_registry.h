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

  friend void RegisterBarbarian(ClassRegistry& registry);
  friend void RegisterBard(ClassRegistry& registry);
  friend void RegisterCleric(ClassRegistry& registry);
  friend void RegisterDruid(ClassRegistry& registry);
  friend void RegisterFighter(ClassRegistry& registry);
  friend void RegisterMonk(ClassRegistry& registry);
  friend void RegisterPaladin(ClassRegistry& registry);
  friend void RegisterRanger(ClassRegistry& registry);
  friend void RegisterRogue(ClassRegistry& registry);
  friend void RegisterSorcerer(ClassRegistry& registry);
  friend void RegisterWarlock(ClassRegistry& registry);
  friend void RegisterWizard(ClassRegistry& registry);

  std::map<ClassType, ClassDefinition> definitions_;
};

void RegisterBarbarian(ClassRegistry& registry);
void RegisterBard(ClassRegistry& registry);
void RegisterCleric(ClassRegistry& registry);
void RegisterDruid(ClassRegistry& registry);
void RegisterFighter(ClassRegistry& registry);
void RegisterMonk(ClassRegistry& registry);
void RegisterPaladin(ClassRegistry& registry);
void RegisterRanger(ClassRegistry& registry);
void RegisterRogue(ClassRegistry& registry);
void RegisterSorcerer(ClassRegistry& registry);
void RegisterWarlock(ClassRegistry& registry);
void RegisterWizard(ClassRegistry& registry);

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_CLASS_REGISTRY_H_

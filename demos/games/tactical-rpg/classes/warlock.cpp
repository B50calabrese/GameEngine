#include "../class_registry.h"

namespace tactical_rpg {

void RegisterWarlock(ClassRegistry& registry) {
  ClassDefinition def;
  def.type = ClassType::Warlock;
  def.name = "Warlock";
  def.base_stats = {8, 12, 14, 10, 10, 16, 8 + 2, 12, 6, 1};
  def.base_actions.push_back("Dagger");
  def.base_actions.push_back("Eldritch Blast");
  registry.RegisterClass(def);
}

}  // namespace tactical_rpg

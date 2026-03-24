#include "../class_registry.h"

namespace tactical_rpg {

void RegisterFighter(ClassRegistry& registry) {
  ClassDefinition def;
  def.type = ClassType::Fighter;
  def.name = "Fighter";
  def.base_stats = {16, 14, 14, 10, 10, 8, 10 + 2, 18, 6, 2};
  def.base_actions.push_back("Longsword");
  def.base_actions.push_back("Second Wind");
  registry.RegisterClass(def);
}

}  // namespace tactical_rpg

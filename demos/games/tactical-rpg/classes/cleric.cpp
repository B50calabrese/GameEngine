#include "../class_registry.h"

namespace tactical_rpg {

void RegisterCleric(ClassRegistry& registry) {
  ClassDefinition def;
  def.type = ClassType::Cleric;
  def.name = "Cleric";
  def.base_stats = {14, 10, 14, 10, 16, 12, 8 + 2, 16, 6, 0};
  def.base_actions.push_back("Mace");
  def.base_actions.push_back("Sacred Flame");
  registry.RegisterClass(def);
}

}  // namespace tactical_rpg

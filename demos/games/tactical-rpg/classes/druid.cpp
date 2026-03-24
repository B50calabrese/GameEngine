#include "../class_registry.h"

namespace tactical_rpg {

void RegisterDruid(ClassRegistry& registry) {
  ClassDefinition def;
  def.type = ClassType::Druid;
  def.name = "Druid";
  def.base_stats = {10, 12, 14, 12, 16, 8, 8 + 2, 14, 6, 1};
  def.base_actions.push_back("Quarterstaff");
  def.base_actions.push_back("Produce Flame");
  registry.RegisterClass(def);
}

}  // namespace tactical_rpg

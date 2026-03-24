#include "../class_registry.h"

namespace tactical_rpg {

void RegisterRanger(ClassRegistry& registry) {
  ClassDefinition def;
  def.type = ClassType::Ranger;
  def.name = "Ranger";
  def.base_stats = {12, 16, 14, 8, 14, 10, 10 + 2, 15, 6, 3};
  def.base_actions.push_back("Longbow");
  def.base_actions.push_back("Hunter's Mark");
  registry.RegisterClass(def);
}

}  // namespace tactical_rpg

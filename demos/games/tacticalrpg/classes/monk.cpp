#include "../class_registry.h"

namespace tactical_rpg {

void RegisterMonk(ClassRegistry& registry) {
  ClassDefinition def;
  def.type = ClassType::Monk;
  def.name = "Monk";
  def.base_stats = {10, 16, 14, 10, 16, 8, 8 + 2, 16, 8, 3};
  def.base_actions.push_back("Unarmed");
  def.base_actions.push_back("Martial Arts");
  registry.RegisterClass(def);
}

}  // namespace tactical_rpg

#include "../class_registry.h"

namespace tactical_rpg {

void RegisterBarbarian(ClassRegistry& registry) {
  ClassDefinition def;
  def.type = ClassType::Barbarian;
  def.name = "Barbarian";
  def.base_stats = {16, 14, 16, 8, 10, 8, 12 + 3, 15, 6, 3};
  def.base_actions.push_back("Greataxe");
  def.base_actions.push_back("Rage");
  registry.RegisterClass(def);
}

}  // namespace tactical_rpg

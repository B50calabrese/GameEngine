#include "../class_registry.h"

namespace tactical_rpg {

void RegisterSorcerer(ClassRegistry& registry) {
  ClassDefinition def;
  def.type = ClassType::Sorcerer;
  def.name = "Sorcerer";
  def.base_stats = {8, 12, 14, 10, 10, 16, 6 + 2, 11, 6, 1};
  def.base_actions.push_back("Quarterstaff");
  def.base_actions.push_back("Firebolt");
  registry.RegisterClass(def);
}

}  // namespace tactical_rpg

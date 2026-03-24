#include "../class_registry.h"

namespace tactical_rpg {

void RegisterWizard(ClassRegistry& registry) {
  ClassDefinition def;
  def.type = ClassType::Wizard;
  def.name = "Wizard";
  def.base_stats = {8, 12, 14, 16, 10, 8, 6 + 2, 11, 6, 1};
  def.base_actions.push_back("Dagger");
  def.base_actions.push_back("Firebolt");
  registry.RegisterClass(def);
}

}  // namespace tactical_rpg

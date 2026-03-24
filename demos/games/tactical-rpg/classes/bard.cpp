#include "../class_registry.h"

namespace tactical_rpg {

void RegisterBard(ClassRegistry& registry) {
  ClassDefinition def;
  def.type = ClassType::Bard;
  def.name = "Bard";
  def.base_stats = {8, 14, 12, 12, 10, 16, 8 + 1, 12, 6, 2};
  def.base_actions.push_back("Rapier");
  def.base_actions.push_back("Vicious Mockery");
  registry.RegisterClass(def);
}

}  // namespace tactical_rpg

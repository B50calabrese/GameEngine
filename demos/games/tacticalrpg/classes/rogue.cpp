#include "../class_registry.h"

namespace tactical_rpg {

void RegisterRogue(ClassRegistry& registry) {
  ClassDefinition def;
  def.type = ClassType::Rogue;
  def.name = "Rogue";
  def.base_stats = {8, 16, 14, 12, 10, 12, 8 + 2, 14, 6, 3};
  def.base_actions.push_back("Shortsword");
  def.base_actions.push_back("Cunning Action");
  registry.RegisterClass(def);
}

}  // namespace tactical_rpg

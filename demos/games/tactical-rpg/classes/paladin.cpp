#include "../class_registry.h"

namespace tactical_rpg {

void RegisterPaladin(ClassRegistry& registry) {
  ClassDefinition def;
  def.type = ClassType::Paladin;
  def.name = "Paladin";
  def.base_stats = {16, 10, 14, 8, 10, 16, 10 + 2, 18, 6, 0};
  def.base_actions.push_back("Warhammer");
  def.base_actions.push_back("Lay on Hands");
  registry.RegisterClass(def);
}

}  // namespace tactical_rpg

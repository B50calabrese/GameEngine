#ifndef DEMOS_GAMES_TACTICAL_RPG_ACTION_REGISTRY_H_
#define DEMOS_GAMES_TACTICAL_RPG_ACTION_REGISTRY_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <engine/ecs/registry.h>

#include "game_types.h"

namespace tactical_rpg {

struct RawAction {
  std::string name;
  ActionType type;
  int range;
  bool is_bonus_action;
  std::string description;
  struct Effect {
    bool is_damage;
    int dice_size;
    int num_dice;
    int modifier;
  };
  std::vector<Effect> effects;
};

class ActionRegistry {
 public:
  static ActionRegistry& Get() {
    static ActionRegistry instance;
    return instance;
  }

  engine::ecs::EntityID GetOrCreateAction(engine::ecs::Registry& registry,
                                          const std::string& name);

  void ClearCache() { action_cache_.clear(); }

 private:
  ActionRegistry() { RegisterAll(); }
  void RegisterAll();

  void RegisterAction(const RawAction& action) { actions_[action.name] = action; }

  std::map<std::string, RawAction> actions_;
  std::map<std::string, engine::ecs::EntityID> action_cache_;
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_ACTION_REGISTRY_H_

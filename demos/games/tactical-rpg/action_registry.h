#ifndef DEMOS_GAMES_TACTICAL_RPG_ACTION_REGISTRY_H_
#define DEMOS_GAMES_TACTICAL_RPG_ACTION_REGISTRY_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "game_types.h"

namespace tactical_rpg {

class ActionRegistry {
 public:
  static ActionRegistry& Get() {
    static ActionRegistry instance;
    return instance;
  }

  void RegisterAction(const Action& action) {
    actions_[action.name] = action;
  }

  Action GetAction(const std::string& name) const {
    auto it = actions_.find(name);
    if (it != actions_.end()) {
      return it->second;
    }
    return {};
  }

 private:
  ActionRegistry() { RegisterAll(); }
  void RegisterAll();

  std::map<std::string, Action> actions_;
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_ACTION_REGISTRY_H_

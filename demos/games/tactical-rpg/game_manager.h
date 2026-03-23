#ifndef DEMOS_GAMES_TACTICAL_RPG_GAME_MANAGER_H_
#define DEMOS_GAMES_TACTICAL_RPG_GAME_MANAGER_H_

#include <memory>
#include <vector>

#include "game_types.h"
#include "map_system.h"

namespace tactical_rpg {

class GameManager {
 public:
  static GameManager& Get() {
    static GameManager instance;
    return instance;
  }

  void StartNewRun(const std::vector<ClassType>& selected_classes);
  void ShowMap();
  void EnterNode(int node_id);
  void CompleteBattle(bool victory);

  std::vector<Character>& GetParty() { return party_; }
  const std::vector<MapNode>& GetMap() { return current_map_; }
  int GetCurrentNodeId() const { return current_node_id_; }

 private:
  GameManager() = default;

  std::vector<Character> party_;
  std::vector<MapNode> current_map_;
  int current_node_id_ = 0;
  int current_floor_ = 0;
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_GAME_MANAGER_H_

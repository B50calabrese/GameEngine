#include "map_node.h"

#include <engine/scene/scene_manager.h>

#include "battle_scene.h"
#include "game_manager.h"

namespace tactical_rpg {

void FightNode::OnEnter() {
  engine::SceneManager::Get().PushScene(std::make_unique<BattleScene>(
      "Battle", GameManager::Get().GetParty(), GetFloor()));
}

void RestNode::OnEnter() {
  for (auto& p : GameManager::Get().GetParty()) {
    p.stats.current_hp =
        std::min(p.stats.max_hp, p.stats.current_hp + p.stats.max_hp / 2);
    p.is_downed = false;
  }
}

void PlaceholderNode::OnEnter() {
  // Just a placeholder
}

}  // namespace tactical_rpg

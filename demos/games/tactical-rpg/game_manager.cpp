#include "game_manager.h"

#include <engine/scene/scene_manager.h>
#include <engine/util/logger.h>

#include "battle_scene.h"
#include "class_registry.h"
#include "main_menu_scene.h"
#include "map_scene.h"
#include "result_scene.h"

namespace tactical_rpg {

void GameManager::StartNewRun(const std::vector<ClassType>& selected_classes) {
  party_.clear();
  for (auto type : selected_classes) {
    party_.push_back(
        ClassRegistry::Get().CreateCharacter(type, ClassTypeToString(type)));
  }

  current_floor_ = 1;
  current_map_ = MapSystem::GenerateMap(10, 3, 12345);
  current_node_id_ = 0;

  ShowMap();
}

void GameManager::ShowMap() {
  engine::SceneManager::Get().SetScene(
      std::make_unique<MapScene>("Map", current_map_, current_node_id_));
}

void GameManager::EnterNode(int node_id) {
  current_node_id_ = node_id;
  auto& node = current_map_[node_id];
  node->OnEnter();

  if (node->GetType() != NodeType::Fight && node->GetType() != NodeType::Boss) {
    ShowMap();
  }
}

void MainMenuScene::StartGame() {
  GameManager::Get().StartNewRun(selected_classes_);
}

void MapScene::EnterNode(int node_id) { GameManager::Get().EnterNode(node_id); }

void GameManager::CompleteBattle(bool victory) {
  if (victory) {
    current_floor_++;
    if (current_floor_ > 10) {
      engine::SceneManager::Get().SetScene(std::make_unique<ResultScene>(
          "Victory", "You reached the end of the dungeon!", true));
    } else {
      // Restore downed characters with some HP
      for (auto& p : party_) {
        if (p.is_downed) {
          p.is_downed = false;
          p.stats.current_hp = 1;
        }
      }
      ShowMap();
    }
  } else {
    engine::SceneManager::Get().SetScene(std::make_unique<ResultScene>(
        "GameOver",
        "Your party was defeated on floor " + std::to_string(current_floor_),
        false));
  }
}

void ResultScene::ReturnToMenu() {
  engine::SceneManager::Get().SetScene(
      std::make_unique<MainMenuScene>("MainMenu"));
}

}  // namespace tactical_rpg

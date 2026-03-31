#ifndef RPG_SCENES_H_
#define RPG_SCENES_H_

#include <string>

#include <glm/vec2.hpp>

#include <engine/scene/scene.h>

#include "game_state.h"
#include "map_generator.h"

namespace rpg {

class MapScene : public engine::Scene {
 public:
  explicit MapScene(const std::string& name);
  void OnAttach() override;
  void OnUpdate(float dt) override;
  void OnRender() override;

 private:
  void GenerateLevel();
  void HandleTile(int x, int y, TileType type);

  MapGenerator::MapData map_;
  glm::ivec2 player_pos_;
};

class BattleScene : public engine::Scene {
 public:
  explicit BattleScene(const std::string& name);
  void OnAttach() override;
  void OnUpdate(float dt) override;
  void OnRender() override;

 private:
  void ExecuteAction();
  void EnemyTurn();
  void RefreshSelection();

  Stats enemy_stats_;
  float flee_difficulty_;
  int selected_index_ = 0;
  int options_count_ = 0;
  std::string last_log_ = "Your Turn";
};

class VictoryOverlay : public engine::Scene {
 public:
  VictoryOverlay();
  void OnUpdate(float dt) override;
  void OnRender() override;
};

class LevelUpOverlay : public engine::Scene {
 public:
  LevelUpOverlay();
  void OnUpdate(float dt) override;
  void OnRender() override;

 private:
  void ExecuteSelection();

  bool stat_picked_ = false;
  int selected_index_ = 0;
  int options_count_ = 0;
  std::vector<Attack> pending_attacks_;
};

}  // namespace rpg

#endif  // RPG_SCENES_H_

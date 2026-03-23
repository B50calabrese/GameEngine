#ifndef DEMOS_GAMES_TACTICAL_RPG_BATTLE_SCENE_H_
#define DEMOS_GAMES_TACTICAL_RPG_BATTLE_SCENE_H_

#include <algorithm>
#include <memory>
#include <random>
#include <vector>

#include <engine/graphics/renderer.h>
#include <engine/graphics/text_renderer.h>
#include <engine/input/input_manager.h>
#include <engine/scene/scene.h>

#include "game_types.h"

namespace tactical_rpg {

class BattleScene : public engine::Scene {
 public:
  BattleScene(const std::string& name, std::vector<Character>& party,
              int difficulty)
      : engine::Scene(name), party_(party), difficulty_(difficulty) {}

  void OnAttach() override;
  void OnUpdate(float dt) override;
  void OnRender() override;

 private:
  void SetupGrid();
  void SetupEnemies();
  void RollInitiative();
  void NextTurn();
  void HandlePlayerTurn(float dt);
  void HandleEnemyAI();
  void CheckVictoryLoss();

  void RenderGrid();
  void RenderCharacters();
  void RenderUI();

  std::vector<Character>& party_;
  std::vector<Character> enemies_;
  int difficulty_;

  // Grid
  static const int kGridSize = 10;
  TerrainType grid_[kGridSize][kGridSize];
  float tile_visual_size_ = 50.0f;
  glm::vec2 grid_offset_ = {100.0f, 100.0f};

  // Turn management
  std::vector<Character*> turn_order_;
  int current_turn_index_ = 0;
  Character* active_character_ = nullptr;

  // Interaction
  glm::ivec2 cursor_pos_ = {0, 0};
  int selected_action_index_ = -1;  // -1 means movement mode

  bool is_battle_over_ = false;
  std::string last_log_ = "Battle Start!";
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_BATTLE_SCENE_H_

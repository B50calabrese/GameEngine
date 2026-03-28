#ifndef DEMOS_GAMES_TACTICAL_RPG_BATTLE_SCENE_H_
#define DEMOS_GAMES_TACTICAL_RPG_BATTLE_SCENE_H_

#include <memory>
#include <vector>

#include <engine/graphics/ecs/text_renderer.h>
#include <engine/graphics/renderer.h>
#include <engine/input/input_manager.h>
#include <engine/scene/scene.h>

#include "battle_grid.h"
#include "game_types.h"
#include "turn_manager.h"

namespace tactical_rpg {

class BattleScene : public engine::Scene {
 public:
  BattleScene(const std::string& name, std::vector<Character>& party,
              int difficulty)
      : engine::Scene(name), party_data_(party), difficulty_(difficulty) {}

  void OnAttach() override;
  void OnUpdate(float dt) override;
  void OnRender() override;

 private:
  void SetupEnemies();
  void HandlePlayerTurn(float dt);
  void HandleEnemyAI();

  void RenderCharacters();
  void RenderUI();

  std::vector<Character>& party_data_;
  std::vector<engine::ecs::EntityID> party_entities_;
  std::vector<engine::ecs::EntityID> enemy_entities_;
  int difficulty_;

  TurnManager turn_manager_;

  float tile_visual_size_ = 50.0f;
  glm::vec2 grid_offset_ = {100.0f, 100.0f};

  glm::ivec2 cursor_pos_ = {0, 0};
  int selected_action_index_ = -1;

  bool is_battle_over_ = false;
  std::string last_log_ = "Battle Start!";
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_BATTLE_SCENE_H_

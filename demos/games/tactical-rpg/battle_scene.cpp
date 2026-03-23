#include "battle_scene.h"

#include <algorithm>
#include <random>

#include <engine/core/engine.h>
#include <engine/graphics/renderer.h>
#include <engine/input/input_manager.h>
#include <engine/scene/scene_manager.h>
#include <engine/util/logger.h>

#include "class_registry.h"
#include "combat_rules.h"
#include "enemy_ai.h"
#include "game_manager.h"

namespace tactical_rpg {

void BattleScene::OnAttach() {
  engine::graphics::TextRenderer::Get().Init();
  engine::graphics::TextRenderer::Get().LoadFont("default", "arial.ttf", 24);

  grid_.Setup();
  SetupEnemies();
  turn_manager_.RollInitiative(party_, enemies_);
  turn_manager_.NextTurn();

  auto* active = turn_manager_.GetActiveCharacter();
  if (active) cursor_pos_ = active->grid_pos;
}

void BattleScene::SetupEnemies() {
  std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<int> class_dist(0, 11);

  enemies_.clear();
  int num_enemies = 2 + (difficulty_ / 2);
  for (int i = 0; i < num_enemies; ++i) {
    ClassType type = static_cast<ClassType>(class_dist(gen));
    Character c = ClassRegistry::Get().CreateCharacter(
        type, "Enemy " + std::to_string(i + 1), true);
    c.grid_pos = {i * 2, BattleGrid::kSize - 1};
    enemies_.push_back(c);
  }

  for (int i = 0; i < (int)party_.size(); ++i) {
    party_[i].grid_pos = {i * 2, 0};
  }
}

void BattleScene::OnUpdate(float dt) {
  if (is_battle_over_) {
    if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kSpace)) {
      bool victory =
          std::all_of(enemies_.begin(), enemies_.end(),
                      [](const Character& c) { return c.is_downed; });
      GameManager::Get().CompleteBattle(victory);
    }
    return;
  }

  auto* active = turn_manager_.GetActiveCharacter();
  if (!active) return;

  if (!active->is_enemy) {
    HandlePlayerTurn(dt);
  } else {
    HandleEnemyAI();
  }

  if (turn_manager_.IsBattleOver(party_, enemies_)) {
    is_battle_over_ = true;
    bool victory = std::all_of(enemies_.begin(), enemies_.end(),
                               [](const Character& c) { return c.is_downed; });
    last_log_ = victory ? "VICTORY! Press SPACE" : "DEFEAT! Press SPACE";
  }
}

void BattleScene::HandlePlayerTurn(float dt) {
  auto& input = engine::InputManager::Get();
  auto* active = turn_manager_.GetActiveCharacter();

  // Movement
  if (input.IsKeyPressed(engine::KeyCode::kW))
    cursor_pos_.y = std::max(0, cursor_pos_.y - 1);
  if (input.IsKeyPressed(engine::KeyCode::kS))
    cursor_pos_.y = std::min(BattleGrid::kSize - 1, cursor_pos_.y + 1);
  if (input.IsKeyPressed(engine::KeyCode::kA))
    cursor_pos_.x = std::max(0, cursor_pos_.x - 1);
  if (input.IsKeyPressed(engine::KeyCode::kD))
    cursor_pos_.x = std::min(BattleGrid::kSize - 1, cursor_pos_.x + 1);

  // Selection
  if (input.IsKeyPressed(engine::KeyCode::k1)) selected_action_index_ = -1;
  if (input.IsKeyPressed(engine::KeyCode::k2)) selected_action_index_ = 0;
  if (input.IsKeyPressed(engine::KeyCode::k3)) selected_action_index_ = 1;

  if (input.IsKeyPressed(engine::KeyCode::kSpace)) {
    if (selected_action_index_ == -1) {
      std::vector<Character> all_chars = party_;
      all_chars.insert(all_chars.end(), enemies_.begin(), enemies_.end());

      if (CombatRules::CanMoveTo(*active, grid_, cursor_pos_.x, cursor_pos_.y,
                                 all_chars)) {
        int dist = std::abs(cursor_pos_.x - active->grid_pos.x) +
                   std::abs(cursor_pos_.y - active->grid_pos.y);
        active->grid_pos = cursor_pos_;
        active->movement_remaining -= dist;

        if (grid_.GetTerrain(cursor_pos_.x, cursor_pos_.y) ==
            TerrainType::Damage) {
          active->stats.current_hp -= 2;
          last_log_ = active->name + " took damage!";
        }
      }
    } else {
      if (selected_action_index_ < (int)active->actions.size()) {
        auto& action = active->actions[selected_action_index_];
        bool can_use = (action.is_bonus_action ? !active->bonus_action_used
                                               : !active->action_used);

        if (can_use) {
          // Find target
          Character* target = nullptr;
          for (auto& p : party_)
            if (!p.is_downed && p.grid_pos == cursor_pos_) target = &p;
          for (auto& e : enemies_)
            if (!e.is_downed && e.grid_pos == cursor_pos_) target = &e;

          if (target &&
              CombatRules::IsInRange(*active, *target, action.range)) {
            std::mt19937 gen(std::random_device{}());
            int roll = std::uniform_int_distribution<int>(1, 20)(gen);

            bool is_heal = (action.name == "Lay on Hands" || action.name == "Second Wind");

            if (is_heal) {
              int heal_amount = action.damage_modifier;
              if (action.name == "Second Wind") heal_amount = std::uniform_int_distribution<int>(1, 10)(gen) + 2;
              target->stats.current_hp = std::min(target->stats.max_hp, target->stats.current_hp + heal_amount);
              last_log_ = active->name + " healed " + target->name + " for " + std::to_string(heal_amount);
            } else if (roll + 5 >= target->stats.ac) {
              int damage = std::uniform_int_distribution<int>(
                               1, action.damage_dice_sides)(gen) *
                               action.damage_dice_count +
                           action.damage_modifier;
              target->stats.current_hp -= damage;
              last_log_ = active->name + " hit " + target->name + " for " +
                          std::to_string(damage);
              if (target->stats.current_hp <= 0) {
                target->is_downed = true;
                target->stats.current_hp = 0;
              }
            } else {
              last_log_ = active->name + " missed!";
            }

            if (action.is_bonus_action)
              active->bonus_action_used = true;
            else
              active->action_used = true;
          }
        }
      }
    }
  }

  if (input.IsKeyPressed(engine::KeyCode::kEnter)) {
    turn_manager_.NextTurn();
    auto* next = turn_manager_.GetActiveCharacter();
    if (next) {
      cursor_pos_ = next->grid_pos;
      last_log_ = next->name + "'s Turn!";
    }
  }
}

void BattleScene::HandleEnemyAI() {
  auto* active = turn_manager_.GetActiveCharacter();
  EnemyAI::ProcessTurn(active, party_);

  // Auto end turn
  turn_manager_.NextTurn();
  auto* next = turn_manager_.GetActiveCharacter();
  if (next) {
    cursor_pos_ = next->grid_pos;
    last_log_ = next->name + "'s Turn!";
  }
}

void BattleScene::OnRender() {
  grid_.Render(grid_offset_, tile_visual_size_, cursor_pos_);
  RenderCharacters();
  RenderUI();
}

void BattleScene::RenderCharacters() {
  for (auto& p : party_) {
    if (p.is_downed) continue;
    glm::vec2 pos = grid_offset_ + glm::vec2(p.grid_pos.x * tile_visual_size_,
                                             p.grid_pos.y * tile_visual_size_);
    engine::graphics::Renderer::Get().DrawQuad(
        pos + glm::vec2(5, 5), {tile_visual_size_ - 12, tile_visual_size_ - 12},
        {0, 1, 0, 1});
  }
  for (auto& e : enemies_) {
    if (e.is_downed) continue;
    glm::vec2 pos = grid_offset_ + glm::vec2(e.grid_pos.x * tile_visual_size_,
                                             e.grid_pos.y * tile_visual_size_);
    engine::graphics::Renderer::Get().DrawQuad(
        pos + glm::vec2(5, 5), {tile_visual_size_ - 12, tile_visual_size_ - 12},
        {1, 0, 0, 1});
  }

  auto* active = turn_manager_.GetActiveCharacter();
  if (active) {
    glm::vec2 pos =
        grid_offset_ + glm::vec2(active->grid_pos.x * tile_visual_size_,
                                 active->grid_pos.y * tile_visual_size_);
    engine::graphics::Renderer::Get().DrawQuad(
        pos, {tile_visual_size_ - 2, tile_visual_size_ - 2}, {1, 1, 0, 0.3f});
  }
}

void BattleScene::RenderUI() {
  engine::graphics::TextRenderer::Get().DrawText("default", last_log_, {100, 650},
                                             0.0f, 1.0f, {1, 1, 1, 1});

  auto* active = turn_manager_.GetActiveCharacter();
  if (active && !active->is_enemy) {
    std::string move_info =
        "Movement: " + std::to_string(active->movement_remaining) + "/" +
        std::to_string(active->stats.speed);
    engine::graphics::TextRenderer::Get().DrawText("default", move_info, {700, 500},
                                               0.0f, 0.7f, {1, 1, 1, 1});

    engine::graphics::TextRenderer::Get().DrawText(
        "default", "1: Move Mode", {700, 450}, 0.0f, 0.7f,
        (selected_action_index_ == -1 ? glm::vec4(1, 1, 0, 1)
                                      : glm::vec4(1, 1, 1, 1)));

    for (int i = 0; i < (int)active->actions.size(); ++i) {
      auto& action = active->actions[i];
      glm::vec4 color = (selected_action_index_ == i ? glm::vec4(1, 1, 0, 1)
                                                     : glm::vec4(1, 1, 1, 1));
      engine::graphics::TextRenderer::Get().DrawText(
          "default", std::to_string(i + 2) + ": " + action.name,
          {700, 400 - i * 40.0f}, 0.0f, 0.7f, color);
    }

    engine::graphics::TextRenderer::Get().DrawText(
        "default", "ENTER: End Turn", {700, 100}, 0.0f, 0.7f, {1, 1, 1, 1});
    engine::graphics::TextRenderer::Get().DrawText(
        "default",
        active->name + " HP: " + std::to_string(active->stats.current_hp) +
            "/" + std::to_string(active->stats.max_hp),
        {100, 50}, 0.0f, 0.7f, {1, 1, 1, 1});
  }

  engine::graphics::Renderer::Get().Flush();
}

}  // namespace tactical_rpg

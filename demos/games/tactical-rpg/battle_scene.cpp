#include "battle_scene.h"

#include <algorithm>
#include <random>

#include <engine/core/engine.h>
#include <engine/graphics/renderer.h>
#include <engine/input/input_manager.h>
#include <engine/scene/scene_manager.h>
#include <engine/util/logger.h>

#include "class_registry.h"
#include "game_manager.h"

namespace tactical_rpg {

void BattleScene::OnAttach() {
  engine::graphics::TextRenderer::Get().Init();
  engine::graphics::TextRenderer::Get().LoadFont("default", "arial.ttf", 24);

  SetupGrid();
  SetupEnemies();
  RollInitiative();
  NextTurn();
}

void BattleScene::SetupGrid() {
  std::mt19937 gen(std::random_device{}());
  std::uniform_real_distribution<float> dist(0.0f, 1.0f);

  for (int y = 0; y < kGridSize; ++y) {
    for (int x = 0; x < kGridSize; ++x) {
      float r = dist(gen);
      if (r < 0.1f)
        grid_[y][x] = TerrainType::Slow;
      else if (r < 0.15f)
        grid_[y][x] = TerrainType::Damage;
      else if (r < 0.2f)
        grid_[y][x] = TerrainType::Impassible;
      else
        grid_[y][x] = TerrainType::Normal;
    }
  }

  // Player and Enemy start areas are always normal
  for (int x = 0; x < kGridSize; ++x) {
    grid_[0][x] = TerrainType::Normal;
    grid_[1][x] = TerrainType::Normal;
    grid_[kGridSize - 1][x] = TerrainType::Normal;
    grid_[kGridSize - 2][x] = TerrainType::Normal;
  }
}

void BattleScene::SetupEnemies() {
  std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<int> class_dist(0, 11);

  int num_enemies = 2 + (difficulty_ / 2);
  for (int i = 0; i < num_enemies; ++i) {
    ClassType type = static_cast<ClassType>(class_dist(gen));
    Character c = ClassRegistry::Get().CreateCharacter(
        type, "Enemy " + std::to_string(i + 1), true);
    c.grid_pos = {i * 2, kGridSize - 1};
    enemies_.push_back(c);
  }

  for (int i = 0; i < (int)party_.size(); ++i) {
    party_[i].grid_pos = {i * 2, 0};
  }
}

void BattleScene::RollInitiative() {
  std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<int> d20(1, 20);

  turn_order_.clear();
  for (auto& p : party_) {
    if (!p.is_downed) {
      p.initiative_roll = d20(gen) + p.stats.initiative_mod;
      turn_order_.push_back(&p);
    }
  }
  for (auto& e : enemies_) {
    e.initiative_roll = d20(gen) + e.stats.initiative_mod;
    turn_order_.push_back(&e);
  }

  std::sort(turn_order_.begin(), turn_order_.end(),
            [](Character* a, Character* b) {
              return a->initiative_roll > b->initiative_roll;
            });

  current_turn_index_ = -1;
}

void BattleScene::NextTurn() {
  current_turn_index_ = (current_turn_index_ + 1) % turn_order_.size();
  active_character_ = turn_order_[current_turn_index_];

  if (active_character_->is_downed) {
    NextTurn();
    return;
  }

  // Reset turn state
  active_character_->movement_remaining = active_character_->stats.speed;
  active_character_->action_used = false;
  active_character_->bonus_action_used = false;

  cursor_pos_ = active_character_->grid_pos;
  selected_action_index_ = -1;

  last_log_ = active_character_->name + "'s Turn!";

  if (active_character_->is_enemy) {
    HandleEnemyAI();
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

  if (!active_character_->is_enemy) {
    HandlePlayerTurn(dt);
  }

  CheckVictoryLoss();
}

void BattleScene::HandlePlayerTurn(float dt) {
  auto& input = engine::InputManager::Get();

  // Movement
  if (input.IsKeyPressed(engine::KeyCode::kW))
    cursor_pos_.y = std::min(kGridSize - 1, cursor_pos_.y + 1);
  if (input.IsKeyPressed(engine::KeyCode::kS))
    cursor_pos_.y = std::max(0, cursor_pos_.y - 1);
  if (input.IsKeyPressed(engine::KeyCode::kA))
    cursor_pos_.x = std::max(0, cursor_pos_.x - 1);
  if (input.IsKeyPressed(engine::KeyCode::kD))
    cursor_pos_.x = std::min(kGridSize - 1, cursor_pos_.x + 1);

  // Selection
  if (input.IsKeyPressed(engine::KeyCode::k1)) selected_action_index_ = -1;
  if (input.IsKeyPressed(engine::KeyCode::k2)) selected_action_index_ = 0;
  if (input.IsKeyPressed(engine::KeyCode::k3)) selected_action_index_ = 1;

  if (input.IsKeyPressed(engine::KeyCode::kSpace)) {
    if (selected_action_index_ == -1) {
      // Move to cursor
      int dist = std::abs(cursor_pos_.x - active_character_->grid_pos.x) +
                 std::abs(cursor_pos_.y - active_character_->grid_pos.y);
      if (dist <= active_character_->movement_remaining &&
          grid_[cursor_pos_.y][cursor_pos_.x] != TerrainType::Impassible) {
        active_character_->grid_pos = cursor_pos_;
        active_character_->movement_remaining -= dist;
        // Check for lava damage
        if (grid_[cursor_pos_.y][cursor_pos_.x] == TerrainType::Damage) {
          active_character_->stats.current_hp -= 2;
          last_log_ = active_character_->name + " took lava damage!";
        }
      }
    } else {
      // Use action
      if (selected_action_index_ < (int)active_character_->actions.size()) {
        auto& action = active_character_->actions[selected_action_index_];
        if ((action.is_bonus_action && !active_character_->bonus_action_used) ||
            (!action.is_bonus_action && !active_character_->action_used)) {
          // Check range
          int dist = std::abs(cursor_pos_.x - active_character_->grid_pos.x) +
                     std::abs(cursor_pos_.y - active_character_->grid_pos.y);

          if (dist <= action.range) {
            // Find target
            Character* target = nullptr;
            for (auto& p : party_)
              if (p.grid_pos == cursor_pos_) target = &p;
            for (auto& e : enemies_)
              if (e.grid_pos == cursor_pos_) target = &e;

            if (target) {
              std::mt19937 gen(std::random_device{}());
              int attack_roll = std::uniform_int_distribution<int>(1, 20)(gen) +
                                5;  // Simplified
              if (attack_roll >= target->stats.ac) {
                int damage = std::uniform_int_distribution<int>(
                                 1, action.damage_dice_sides)(gen) *
                                 action.damage_dice_count +
                             action.damage_modifier;
                target->stats.current_hp -= damage;
                last_log_ = active_character_->name + " hit " + target->name +
                            " for " + std::to_string(damage);
                if (target->stats.current_hp <= 0) {
                  target->is_downed = true;
                  target->stats.current_hp = 0;
                  last_log_ += ". " + target->name + " is downed!";
                }
              } else {
                last_log_ = active_character_->name + " missed " + target->name;
              }

              if (action.is_bonus_action)
                active_character_->bonus_action_used = true;
              else
                active_character_->action_used = true;
            }
          }
        }
      }
    }
  }

  if (input.IsKeyPressed(engine::KeyCode::kEnter)) {
    NextTurn();
  }
}

void BattleScene::HandleEnemyAI() {
  // Basic AI: Move towards closest player and attack
  Character* closest_player = nullptr;
  int min_dist = 1000;

  for (auto& p : party_) {
    if (!p.is_downed) {
      int dist = std::abs(p.grid_pos.x - active_character_->grid_pos.x) +
                 std::abs(p.grid_pos.y - active_character_->grid_pos.y);
      if (dist < min_dist) {
        min_dist = dist;
        closest_player = &p;
      }
    }
  }

  if (closest_player) {
    // Move towards
    glm::ivec2 diff = closest_player->grid_pos - active_character_->grid_pos;
    int move_x = (diff.x > 0)
                     ? std::min(active_character_->stats.speed, diff.x)
                     : std::max(-active_character_->stats.speed, diff.x);
    // Simplified move
    active_character_->grid_pos.x += move_x;

    // Attack if in range
    auto& action = active_character_->actions[0];
    int final_dist =
        std::abs(closest_player->grid_pos.x - active_character_->grid_pos.x) +
        std::abs(closest_player->grid_pos.y - active_character_->grid_pos.y);
    if (final_dist <= action.range) {
      std::mt19937 gen(std::random_device{}());
      int attack_roll = std::uniform_int_distribution<int>(1, 20)(gen) + 2;
      if (attack_roll >= closest_player->stats.ac) {
        int damage = std::uniform_int_distribution<int>(
                         1, action.damage_dice_sides)(gen) *
                         action.damage_dice_count +
                     action.damage_modifier;
        closest_player->stats.current_hp -= damage;
        last_log_ = active_character_->name + " hit " + closest_player->name +
                    " for " + std::to_string(damage);
        if (closest_player->stats.current_hp <= 0) {
          closest_player->is_downed = true;
          closest_player->stats.current_hp = 0;
        }
      }
    }
  }

  // Automatically end enemy turn after delay or immediately for now
  NextTurn();
}

void BattleScene::CheckVictoryLoss() {
  bool all_enemies_down =
      std::all_of(enemies_.begin(), enemies_.end(),
                  [](const Character& c) { return c.is_downed; });
  bool all_party_down =
      std::all_of(party_.begin(), party_.end(),
                  [](const Character& c) { return c.is_downed; });

  if (all_enemies_down) {
    is_battle_over_ = true;
    last_log_ = "VICTORY! Press SPACE to continue.";
  } else if (all_party_down) {
    is_battle_over_ = true;
    last_log_ = "DEFEAT! Press SPACE to return to menu.";
  }
}

void BattleScene::OnRender() {
  RenderGrid();
  RenderCharacters();
  RenderUI();
}

void BattleScene::RenderGrid() {
  for (int y = 0; y < kGridSize; ++y) {
    for (int x = 0; x < kGridSize; ++x) {
      glm::vec2 pos = grid_offset_ +
                      glm::vec2(x * tile_visual_size_, y * tile_visual_size_);
      glm::vec4 color;
      switch (grid_[y][x]) {
        case TerrainType::Normal:
          color = {0.2f, 0.2f, 0.2f, 1.0f};
          break;
        case TerrainType::Slow:
          color = {0.4f, 0.4f, 0.2f, 1.0f};
          break;
        case TerrainType::Damage:
          color = {0.5f, 0.1f, 0.1f, 1.0f};
          break;
        case TerrainType::Impassible:
          color = {0.1f, 0.1f, 0.1f, 1.0f};
          break;
      }

      // Cursor
      if (x == cursor_pos_.x && y == cursor_pos_.y) {
        color += glm::vec4(0.3f, 0.3f, 0.3f, 0.0f);
      }

      engine::graphics::Renderer::Get().DrawQuad(
          pos, {tile_visual_size_ - 2, tile_visual_size_ - 2}, color);
    }
  }
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

  // Highlight active
  if (active_character_) {
    glm::vec2 pos =
        grid_offset_ +
        glm::vec2(active_character_->grid_pos.x * tile_visual_size_,
                  active_character_->grid_pos.y * tile_visual_size_);
    engine::graphics::Renderer::Get().DrawQuad(
        pos, {tile_visual_size_ - 2, tile_visual_size_ - 2}, {1, 1, 0, 0.3f});
  }
}

void BattleScene::RenderUI() {
  engine::graphics::Renderer::Get().DrawText("default", last_log_, {100, 650},
                                             0.0f, 1.0f, {1, 1, 1, 1});

  if (active_character_ && !active_character_->is_enemy) {
    std::string move_info =
        "Movement: " + std::to_string(active_character_->movement_remaining) +
        "/" + std::to_string(active_character_->stats.speed);
    engine::graphics::Renderer::Get().DrawText("default", move_info, {700, 500},
                                               0.0f, 0.7f, {1, 1, 1, 1});

    engine::graphics::Renderer::Get().DrawText(
        "default", "1: Move Mode", {700, 450}, 0.0f, 0.7f,
        (selected_action_index_ == -1 ? glm::vec4(1, 1, 0, 1)
                                      : glm::vec4(1, 1, 1, 1)));

    for (int i = 0; i < (int)active_character_->actions.size(); ++i) {
      auto& action = active_character_->actions[i];
      glm::vec4 color = (selected_action_index_ == i ? glm::vec4(1, 1, 0, 1)
                                                     : glm::vec4(1, 1, 1, 1));
      engine::graphics::Renderer::Get().DrawText(
          "default", std::to_string(i + 2) + ": " + action.name,
          {700, 400 - i * 40.0f}, 0.0f, 0.7f, color);
    }

    engine::graphics::Renderer::Get().DrawText(
        "default", "ENTER: End Turn", {700, 100}, 0.0f, 0.7f, {1, 1, 1, 1});

    // Stats of hovered or active
    engine::graphics::Renderer::Get().DrawText(
        "default",
        active_character_->name +
            " HP: " + std::to_string(active_character_->stats.current_hp) +
            "/" + std::to_string(active_character_->stats.max_hp),
        {100, 50}, 0.0f, 0.7f, {1, 1, 1, 1});
  }

  engine::graphics::Renderer::Get().Flush();
}

}  // namespace tactical_rpg
